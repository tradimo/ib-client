#include "client.h"
#include "contracts.h"

#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>
#include <memory>

// constructor
CClient::CClient() 
	: m_pClientSocket(new EClientSocket(this, &m_readerOSSignal))
{}

bool CClient::connect(std::string addr, int port)
{
	printf("Connecting to %s:%d... ", addr.empty() ? "127.0.0.1" : addr.c_str(), port);
	
	bool res = m_pClientSocket->eConnect(addr.c_str(), port); //, clientId, m_extraAuth)
	
	if (res) {
		printf("SUCCESS\nConnected to %s:%d\n", m_pClientSocket->host().c_str(), m_pClientSocket->port());
		m_pReader = std::make_unique<EReader>(m_pClientSocket.get(), &m_readerOSSignal);
		m_pReader->start();
	}
	else
		printf("FAILURE\nCannot connect to %s:%d\n", m_pClientSocket->host().c_str(), m_pClientSocket->port());
	
	return res;
}

void CClient::processMessages(void)
{

	switch (m_state) {
	case state::contractDetails: 
		printf("--> contractDetails:\n");
		m_pClientSocket->reqContractDetails(101, CContracts::simpleFuture());
		m_pClientSocket->reqContractDetails(102, CContracts::crudeOil());
		m_state = state::idle;
		break;

	case state::continuoseFuture: { // Error. Id: 202, Code: 162, Msg: Historical Market Data Service error message:No market data permissions for NYMEX FUT
		printf("--> continuoseFuture:\n");
		m_pClientSocket->reqContractDetails(201, CContracts::continuoseFuture());

		char queryTime[80];
		std::time_t now = std::time(nullptr);
		std::strftime(queryTime, 80, "%Y%m%d %H:%M:%S", std::localtime(&now));

		m_pClientSocket->reqHistoricalData(202, CContracts::continuoseFuture(), queryTime, "1 M", "1 day", "TRADES", 1, 1, false, TagValueListSPtr());
		std::this_thread::sleep_for(std::chrono::seconds(10));
		m_pClientSocket->cancelHistoricalData(202);

		m_state = state::idle;
		break; }

	case state::headTimespamp:
		printf("--> headTimespamp:\n");
		m_pClientSocket->reqHeadTimestamp(301, CContracts::crudeOil(), "TRADES", 1, 1);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		m_pClientSocket->cancelHeadTimestamp(301);
		m_state = state::idle;
		break;

	case state::historicalData: {
		printf("--> historicalData:\n");
		
		char queryTime[80];
		std::time_t now = std::time(nullptr);
		std::strftime(queryTime, 80, "%Y%m%d %H:%M:%S", std::localtime(&now));

		m_pClientSocket->reqHistoricalData(401, CContracts::eurGbpFx(), queryTime, "1 M", "1 day", "MIDPOINT", 1, 1, false, TagValueListSPtr());
		// Error.Id: 402, Code : 162, Msg : Historical Market Data Service error message : No market data permissions for HEX STK
		m_pClientSocket->reqHistoricalData(402, CContracts::europeanStock(), queryTime, "10 D", "1 min", "TRADES", 1, 1, false, TagValueListSPtr());
		std::this_thread::sleep_for(std::chrono::seconds(2));
		m_pClientSocket->cancelHistoricalData(401);
		// m_pClientSocket->cancelHistoricalData(402);
		m_state = state::idle;
		break; }

	case state::delayedTickData:
		printf("--> delayedTickData:\n");
		
		m_pClientSocket->reqMarketDataType(4);	// send delayed-frozen (4) market data type
		m_pClientSocket->reqMktData(501, CContracts::crudeOil(), "", false, false, TagValueListSPtr());
		std::this_thread::sleep_for(std::chrono::seconds(10));
		m_pClientSocket->cancelMktData(501);
		
		m_state = state::idle;
		break;

	case state::idle: break;
	case state::connect: break;
	}

	m_readerOSSignal.waitForSignal();
	errno = 0;
	m_pReader->processMsgs();
}

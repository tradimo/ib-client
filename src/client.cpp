#include "client.h"

// constructor
CClient::CClient() 
	: m_pClientSocket(new EClientSocket(this))
	, m_pReader(nullptr)
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
	// do job

	m_readerOSSignal.waitForSignal();
	errno = 0;
	m_pReader->processMsgs();
}
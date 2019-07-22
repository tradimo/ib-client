#include "client.h"

// constructor
CClient::CClient() : m_pClientSocket(new EClientSocket(this))
{}

bool CClient::connect(std::string addr, int port)
{
	// trying to connect
//	printf( "Connecting to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientId);
	
	bool res = m_pClientSocket->eConnect(addr.c_str(), port); //, clientId, m_extraAuth)
	
//	if (res) {
//		printf( "Connected to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);
//		m_pReader = new EReader(m_pClient, &m_osSignal);
//		m_pReader->start();
//	}
//	else
//		printf( "Cannot connect to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);
	
	return res;
}

#pragma once

#include "EWrapper.h"
#include "EClientSocket.h"

class CClient : public EWrapper
{
public:
	CClient(void);
	
	bool connect(std::string addr, int port);
	bool isConnected(void) const { return true; }
	
	
private:
	std::unique_ptr<EClientSocket> m_pClientSocket;
};

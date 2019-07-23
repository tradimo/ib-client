#include <thread>
#include <chrono>
#include "stdio.h"

#include "client.h"

int main(int argc, char* argv[])
{
	std::string addr = argc > 1 ? argv[1] : "";
	int port = argc > 2 ? atoi(argv[2]) : 7496;
//	const char* connectOptions = argc > 3 ? argv[3] : "";
	
//	int clientId = 0;
	
	printf("Start of C++ Socket Client Test\n");
	
	for (int attempt = 0; attempt < 50; attempt++) {
		printf( "Attempt %u of %u\n", attempt, 50);
		
		CClient client;
		
		//if (connectOptions) client.setConnectOptions(connectOptions);
		
		client.connect(addr, port);
		
		while (client.isConnected()) 
			client.processMessages();
		
		printf( "Sleeping %u seconds before next attempt\n", 10);
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
	
	printf ("End of C++ Socket Client Test\n");

}

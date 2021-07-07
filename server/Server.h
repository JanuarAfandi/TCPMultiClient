#pragma once 
#include <iostream>
#include <sstream>
#include "Handler.h"
#include <WS2tcpip.h> //library untuk winsock
#pragma comment(lib,"WS2_32")

class Server {
private:
	WSAData wsaData;
	WORD version;

	SOCKET listening;
	sockaddr_in myAddr;

	std::list<SOCKET> socketList;

	int port;

	int clientCount;
	ServerHandle* handler;

	void AcceptNewClient();

public:
	Server();
	Server(int verA, int verB, int Port);

	int Initialize();
	void Start();
};

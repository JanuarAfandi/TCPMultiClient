#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <WS2tcpip.h> //library untuk winsock
#include <climits>
#include <fstream>
#pragma comment (lib, "ws2_32.lib") 
using namespace std;

class Client {
private:
	std::string ipAddress;
	int port;

	WSAData wsaData;
	WORD version;

	SOCKET mysocket;
	sockaddr_in myAddr;
	int Count;
	int byteSize;
	char* data;

	void Recieve();
	void Send();

public:
	Client();
	Client(std::string IpAddress, int Port);

	int Initialize();
	void Start();
};

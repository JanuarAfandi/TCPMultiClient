#include "Server.h"

Server::Server() {
	// pakai versi 2.2
	version = MAKEWORD(2, 2);
	// Port
	port = 8888;
}
Server::Server(int verA, int verB, int Port) {
	version = MAKEWORD(verA, verB);

	port = Port;
}

int Server::Initialize() {
	// Winsock
	int wsaOk = WSAStartup(version, &wsaData);
	if (wsaOk != 0)
	{
		std::cout << "Can`t Initialize Winsock! Exit Program!\n";
		return 1;
	}

	//buat socket
	listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		std::cout << "Can`t create a socket! Exit Program!\n";
		return 1;
	}

	// Bind socket ke ip address and port ke socket
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(port);
	myAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&myAddr, sizeof(myAddr));

	//listening from socket
	listen(listening, SOMAXCONN);

	// Client total set 0
	clientCount = 0;

	return 0;
}

void Server::Start() {
	std::cout << " >> Server Mulai!\n";

	// Mulai menerima klien
	std::thread acceptClient(&Server::AcceptNewClient, this);
	acceptClient.join();
	handler->JoinThread();

	closesocket(listening);
}

void Server::AcceptNewClient() {
	while (true) {
		clientCount++;

		// menerima klien baru
		SOCKET newClient = accept(listening, nullptr, nullptr);
		// tambahkan klien baru ke list
		socketList.push_back(newClient);

		// sapaan kepada klien baru
		std::string temp = "Selamat Datang Client!";
		send(newClient, temp.c_str(), temp.size() + 1, 0);

		// Print pesan jika ada klien yang masuk
		std::cout << " >> Socket " << newClient << " connected to server || ID : " << clientCount << "\n";

		// sapaan kepada klien yang lain
		std::stringstream mySS;
		mySS << "Client " << newClient << " || ID : " << clientCount;
		std::string tempOut = mySS.str();
		for (SOCKET outSock : socketList)
		{

			if (outSock != listening && outSock != newClient)
			{
				send(outSock, tempOut.c_str(), tempOut.size() + 1, 0);
			}
		}

		//mulai handle
		handler = new ServerHandle(newClient, &socketList, clientCount);
		handler->StartHandle();

		newClient = NULL;
	}
}


int main() {
	Server server;
	server.Initialize();
	server.Start();

	return 0;
}
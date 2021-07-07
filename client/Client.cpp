#include "Client.h"

Client::Client() {
	ipAddress = "127.0.0.1";
	port = 8888;
	byteSize = 4056;
	data = new char[byteSize];
}
Client::Client(std::string IpAddress, int Port) {
	ipAddress = IpAddress;
	port = Port;

	byteSize = 4056;
	data = new char[byteSize];
}

int Client::Initialize() {
	version = MAKEWORD(2, 2);
	int wsResult = WSAStartup(version, &wsaData);
	if (wsResult != 0)
	{
		std::cout << "Tidak bisa memulai Winsock! \nError : " << wsResult << "\n Program Keluar";
		return 1;
	}

	mysocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mysocket == INVALID_SOCKET)
	{
		std::cout << "Tidak bisa membuat socket! \nError : " << WSAGetLastError() << "\n Program Keluar";
		WSACleanup();
		return 1;
	}


	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &myAddr.sin_addr);

	// Connect to server
	int connResult = connect(mysocket, (sockaddr*)&myAddr, sizeof(myAddr));
	// Jika gagal connect
	int attemp = 1;
	Count++;
	while (connResult == SOCKET_ERROR)
	{
		system("cls");
		std::cout << "Waiting for Connect " << attemp << " ...";
		attemp++;
		int connResult = connect(mysocket, (sockaddr*)&myAddr, sizeof(myAddr));
		Sleep(1000);
	}
	// Print pesan jika sudah konek
	std::cout << " >> Connected! ";

	return 0;
}

void Client::Start() {
	// Memulai thread pada klien untuk menerima data dari server
	std::thread recieveThread(&Client::Recieve, this);
	std::thread sendThread(&Client::Send, this);
	sendThread.join();
	recieveThread.join();
}

void Client::Recieve() {
	while (true)
	{
		ZeroMemory(data, byteSize);
		int bytesReceived = recv(mysocket, data, byteSize, 0);
		if (bytesReceived > 0)
		{
			std::cout << "\n";
			std::cout << " >> " << std::string(data, 0, bytesReceived) << "\n"; //untuk menerima pesan dari klien
			std::cout << " >> ";
		}
	}
}

void Client::Send() { //mengirim pesan

	std::string userInput;
	while (true)
	{
		std::cin >> userInput;
		std::cout << " >> ";
		send(mysocket, userInput.c_str(), userInput.size() + 1, 0);
	}
}

int main() {
	Client client;
	client.Initialize();
	client.Start();
}
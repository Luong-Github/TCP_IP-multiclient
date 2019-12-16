#include <iostream>
#include <WS2tcpip.h>
#include <string>
#pragma comment (lib, "ws2_32.lib")


int main() {
	using namespace std;
	string ipAddress = "127.0.0.1"; // ipAddress of server
	int port = 2000; // listening port on the server

	// init
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can not start winsock, err " << wsResult << endl;
		return 0;
	}

	// create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "Can not create socket, err " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}

	// fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	
	// connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));

	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can not connect to server, err " << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return 0;
	}
	
	// do while loop to send and receive data
	char buf[4096];
	string userinput;

	do {
		// prompt the user for some text
		getline(cin, userinput);
		if (userinput.size() > 0) {
			// send message
			int sendResult = send(sock, userinput.c_str(), userinput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					// echo 
					cout << "SERVER " << string(buf, 0, bytesReceived) << endl;
				}
			}
		}
	}while (userinput.size() > 0);
	// gracefully close down 
	closesocket(sock);
	WSACleanup();
	system("pause");
}
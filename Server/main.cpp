#include<iostream>
#include<string>
#include<winsock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[error is detected]: " << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//winsock을 초기화
		ShowErrorMessage("socket()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(serverPort);

	if (bind(serverSocket, (SOCKADDR*)& serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "현재상태: bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "현재상태:accept()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)& clientAddress, &sizeClientAddress);

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accpept()");

	while (1)
	{
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "클라이언트 메시지: " << received << '\n';
		cout << "메시지 전송: " << received << '\n';

		if (strcmp(received, "[exit]") == 0)
		{
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "서버종료\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}
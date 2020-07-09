#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include<WinSock2.h>
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>

#define PORT 80 // Port:80 is used by web servers

const char* szHost = "www.google.com";

int main(int argc, char** argv) {
	// Init WINSOCK
	WSAData wsaData;
	WORD Dllversion = MAKEWORD(2, 1); // 1 WORD = 2 Bytes
	if (WSAStartup(Dllversion, &wsaData) != 0) {
		printf("ERROR: Failed to initialize WINSOCK\n");
		ExitProcess(EXIT_FAILURE);
	}

	// Create SOCKET
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		printf("ERROR: Failed to create WINSOCK\n");
		ExitProcess(EXIT_FAILURE);
	}

	// Get server info
	HOSTENT* host = gethostbyname(szHost);
	if (host == nullptr) {
		printf("ERROR: Failed to get server info\n");
		ExitProcess(EXIT_FAILURE);
	}

	// Define server info
	SOCKADDR_IN sin;
	ZeroMemory(&sin, sizeof(sin)); // Not going to use all bytes in the SOCKADDR_IN structure
	sin.sin_port = htons(PORT); // big-endian for network byte order
	sin.sin_family = AF_INET;
	memcpy(&sin.sin_addr.S_un.S_addr, host->h_addr_list[0], sizeof(sin.sin_addr.S_un.S_addr));

	// Connect to the server
	if(connect(sock, (const sockaddr*)&sin, sizeof(sin)) != 0) {
		printf("ERROR: Failed to connect the server\n");
		ExitProcess(EXIT_FAILURE);
	}

	// Send msg to the server (request) 
	const char* szMsg = "HEAD / HTTP/1.0\r\n\r\n";
	if(!send(sock, szMsg, strlen(szMsg), 0)) {
		printf("ERROR: Failed to send request to the server\n");
		ExitProcess(EXIT_FAILURE);
	}

	// Receive data from the server
	char szBuffer[4096];
	char szTemp[4096];
	while (recv(sock, szTemp, 4096, 0)) {
		strcat(szBuffer, szTemp);
	}
	printf("%s\n", szBuffer);

	// Finishing...
	closesocket(sock);
	getchar();
	ExitProcess(EXIT_SUCCESS);

	return 0;
}

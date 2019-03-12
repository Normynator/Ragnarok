#include "stdafx.h"
#include "debug_socket.h"
#include <ws2tcpip.h>
#include "client_ver.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace norm_dll {

	debug_socket::debug_socket()
	{
	}

	debug_socket::debug_socket(std::string ip, std::string port) : ip(ip), port(port)
	{
	}

	debug_socket::~debug_socket()
	{
	}

	SOCKET* debug_socket::get_sock() {
		return &ConnectSocket;
	}

	int debug_socket::do_send(const char* sendbuf)
	{
#ifndef DEBUG
		return 0;
#endif	
		if (this->disabled)
			return 0;

		if (sendbuf) {
			char len = strlen(sendbuf);
			int res = send(ConnectSocket, &len, 1, 0);
			if (res == SOCKET_ERROR) {
				MessageBox(0, (LPCWSTR)"send failed with error!", (LPCWSTR)"norm.dll error!", MB_OK);
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}

			res = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
			if (res == SOCKET_ERROR) {
				MessageBox(0, (LPCWSTR)"send failed with error!", (LPCWSTR)"norm.dll error!", MB_OK);
				//printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}
		}
		return 1;
	}

	int debug_socket::do_connect()
	{
#ifndef DEBUG
		return 0;
#endif
		WSADATA wsaData;
		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;
		int iResult;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			//printf("WSAStartup failed with error: %d\n", iResult);
			MessageBox(0, (LPCWSTR)"WSAStartup failed with error!", (LPCWSTR)"norm.dll error!", MB_OK);
			return 1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		iResult = getaddrinfo(this->ip.c_str(), this->port.c_str(), &hints, &result);
		if (iResult != 0) {
			//printf("getaddrinfo failed with error: %d\n", iResult);
			MessageBox(0, (LPCWSTR)"getaddrinfo failed with error!", (LPCWSTR)"norm.dll error!", MB_OK);
			WSACleanup();
			return 1;
		}

		ptr = result;
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			//printf("socket failed with error: %ld\n", WSAGetLastError());
			MessageBox(0, (LPCWSTR)"socket failed with error!", (LPCWSTR)"norm.dll error!", MB_OK);
			WSACleanup();
			return 1;
		}

			// Connect to server.
		iResult = connect(this->ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
		}

		if (ConnectSocket == INVALID_SOCKET) {
			//printf("Unable to connect to server!\n");
			MessageBox(0, (LPCWSTR)"Unable to connect to server!!", (LPCWSTR)"norm.dll error!", MB_OK);
			WSACleanup();
			return 1;
		}

		return 0;
	}
}


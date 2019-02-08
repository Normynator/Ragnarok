#pragma once
#include <string>
#include <winsock2.h>

namespace norm_dll {
class debug_socket
{
public:
	debug_socket();
	debug_socket(std::string ip, std::string port);
	virtual ~debug_socket();

	int do_connect();
	int do_send(const char* sendbuf);

private: 
	std::string port = "1337";
	std::string	ip = "127.0.0.1";
	SOCKET ConnectSocket = INVALID_SOCKET;
};
}


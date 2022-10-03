#include <iostream>

#include "Socket_Implementation.h"


int main()
{
	LNET_CREATE_LOG_LEVEL("WINDOWS_SOCKET_LOG_LEVEL");

//	WSADATA wsaData;
//	int error = WSAStartup(MAKEWORD(2,2), &wsaData);
//	if (error != 0) {
//		std::cout << "WSAStartup failed: " << error << "\n";
//		return 1;
//	}

	std::cout << "openning socket!\n";
	LNet::Server_Socket_Ptr server_socket = LNet::Server_Socket_Impl::create(25565);
	if(!server_socket)
	{
		std::cout << "error in openning socket!\nerror: " << strerror(errno) << "\n";
		return 1;
	}
	std::cout << "socket openned!\n";

	std::cout << "awaiting connection!\n";
	LNet::Client_Socket_Ptr connected_to = server_socket->wait_for_connection();
	std::cout << "somebody connected!\n";

	while(true)
	{
		LNet::Message msg = connected_to->listen_to_message();
		if(msg.type == LNet::Message::Type::error)
		{
			std::cout << "error occured\n";
			return 0;
		}
		if(msg.type == LNet::Message::Type::disconnect)
		{
			std::cout << "disconnected\n";
			return 0;
		}
		if(msg.type == LNet::Message::Type::message)
			std::cout << msg.message << "\n";
	}

	return 0;
}

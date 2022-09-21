#include <iostream>

#include "Socket_Implementation.h"


/*
#include <thread>
#include <chrono>

#ifdef LNET_ASSERT
	#undef LNET_ASSERT
#endif
#define LNET_ASSERT(condition) if(!condition) { int a = 1; a /= 0; } 1 == 1

void shutdown_connection(int _socket, int _delay)
{
	std::this_thread::sleep_for(std::chrono::seconds(_delay));
	std::cout << "shutdown\n";
	shutdown(_socket, SHUT_RD);
}

void error(const char *msg)
{
	std::cout << msg << "\nerror code: " << errno << "\nerror: " << strerror(errno) << "\n";
	LNET_ASSERT(false);
}

int main()
{
	 int sockfd, newsockfd, portno;
	 socklen_t clilen;
	 char buffer[256];
	 struct sockaddr_in serv_addr, cli_addr;
	 int n;

	 sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 if (sockfd < 0)
		error("ERROR opening socket");
	 bzero((char *) &serv_addr, sizeof(serv_addr));

	 portno = 25565;

	 serv_addr.sin_family = AF_INET;
	 serv_addr.sin_addr.s_addr = INADDR_ANY;
	 serv_addr.sin_port = htons(portno);
	 if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
			  error("ERROR on binding");
	 listen(sockfd,5);
	 clilen = sizeof(cli_addr);

	 std::thread shutdown_thread(shutdown_connection, sockfd, 2);

	 std::cout << "waiting for connection\n";

	 newsockfd = accept(sockfd,
				 (struct sockaddr *) &cli_addr,
				 &clilen);


	 shutdown_thread.join();

	 std::cout << "socket should be shut down\n";

	 std::cout << "\nsocket result: " << newsockfd << "\n\n";

	 std::cout << "attemting to connect again\n";
	 listen(sockfd,1);
	 newsockfd = accept(sockfd,
				 (struct sockaddr *) &cli_addr,
				 &clilen);

	 if (newsockfd < 0)
		  error("ERROR on accept");
	 bzero(buffer,256);
	 n = read(newsockfd,buffer,255);
	 if (n < 0) error("ERROR reading from socket");

	 printf("Here is the message: %s\n",buffer);

	 n = write(newsockfd,"I got your message",18);
	 if (n < 0) error("ERROR writing to socket");
	 close(newsockfd);
	 close(sockfd);

	 return 0;
} */

int main()
{
	LNET_CREATE_LOG_LEVEL("WINDOWS_SOCKET_LOG_LEVEL");

	WSADATA wsaData;
	int error = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (error != 0) {
		std::cout << "WSAStartup failed: " << error << "\n";
		return 1;
	}

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

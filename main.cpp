#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifdef LNET_ASSERT
	#undef LNET_ASSERT
#endif
#define LNET_ASSERT(condition) if(!condition) { int a = 1; a /= 0; } 1 == 1

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
}

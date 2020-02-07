#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define port "3491"

void error(char * msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char const *argv[])
{
	int sockfd, newsockfd;
	char buffer[200];
	char message[200];
	socklen_t clilen;
	struct sockaddr_storage cli_addr;
	struct addrinfo hints, *res;

	memset(&hints,0,sizeof(hints));
	hints.ai_family =AF_UNSPEC;
	hints.ai_socktype =SOCK_DGRAM;
	hints.ai_flags= AI_PASSIVE; //use any ip

	if(getaddrinfo(NULL,port,&hints,&res)==-1){///
	error("addrinfo");
	}

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0)
		error("Error in opening the socket");

	if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
	{
		error("Error in binding");
	}
	else
		printf("binding successful...\n");


	clilen = sizeof(cli_addr);	

	printf("waiting to recvfrom...\n");

	if (recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&cli_addr,&clilen) < 0)
	{
		error("Error in receiving from client1");
	}

	printf("client1: %s\n",buffer);

	if (buffer[0] == 'a')
	{
		buffer[0] = 'z';
	}
	else if (buffer[0] == 'A')
	{
		buffer[0] = 'Z';
	}
	else
		buffer[0] = buffer[0] - 1;

	if (recvfrom(sockfd,message,sizeof(message),0,(struct sockaddr*)&cli_addr,&clilen) < 0)
	{
		error("Error in ping from client2");
	}

	if (sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cli_addr, clilen) < 0) 
	{
		error("Error in sending to client2");
	}

	freeaddrinfo(res);
	close(sockfd);
	

	return 0;
}

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
	socklen_t clilen;
	struct sockaddr_storage cli_addr;
	struct addrinfo hints, *res;

	memset(&hints,0,sizeof(hints));
	hints.ai_family =AF_UNSPEC;
	hints.ai_socktype =SOCK_STREAM;
	hints.ai_flags= AI_PASSIVE; 

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

	listen(sockfd, 2);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd < 0)
		error("Error in accept");
	else
		printf("client connected!!\n");

	while(1) {

		if (recv(newsockfd, buffer, sizeof(buffer), 0) < 0)
			error("Error in receiving from client");

		printf("client: %s", buffer);

		if(strncmp("bye", buffer, 3) == 0){
			printf("\nclient disconnected!!\n");
		}

		fgets(buffer, sizeof(buffer), stdin);

		if (send(newsockfd, buffer, sizeof(buffer), 0) < 0)
			error("Error in sending msg");

	}

	close(newsockfd);
	close(sockfd);
	

	return 0;
}

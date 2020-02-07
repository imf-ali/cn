#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netdb.h>
#define port "3491"

void error(char * msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char const *argv[])
{
	if(argc < 2)
		error("Hostname is required");
	int sockfd;
	char buffer[200];
	struct addrinfo hints, *res;

	memset(&hints,0,sizeof(hints));
	hints.ai_family =AF_UNSPEC;
	hints.ai_socktype =SOCK_STREAM;

	if(getaddrinfo(argv[1],port,&hints,&res)==-1){ 
		error("addrinfo");
	}

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(sockfd < 0)
		error("Error in opening the socket");

	if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0)
	{
		error("Error in connecting");
	}
	else
		printf("connected to server\n");


	while(1) {
		fgets(buffer, sizeof(buffer), stdin);

		if (send(sockfd, buffer, sizeof(buffer), 0) < 0)
			error("Error while sending");

		if (strncmp("bye", buffer, 3) == 0)
			break;

		if(recv(sockfd, buffer, sizeof(buffer), 0) < 0)
			error("Error while receiving");

		printf("Server: %s", buffer);

		if (strncmp("bye", buffer, 3) == 0){
			printf("connection lost\n");
			break;
		}
	}

	close(sockfd);

	return 0;
}

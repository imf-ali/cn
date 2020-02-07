#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
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
	hints.ai_socktype =SOCK_DGRAM;

	if(getaddrinfo(argv[1],port,&hints,&res)==-1){ 
		error("Errot in addrinfo");
	}

	sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(sockfd < 0)
		error("Error in opening the socket");

	printf("Enter a character: ");
	scanf("%s",buffer);

	if (sendto(sockfd, buffer, sizeof(buffer), 0, res->ai_addr, res->ai_addrlen) < 0) 
	{
		error("Error in sending");
	}

	freeaddrinfo(res);
	printf("sent %s to %s\n",buffer,argv[1]);

	close(sockfd);

	return 0;
}


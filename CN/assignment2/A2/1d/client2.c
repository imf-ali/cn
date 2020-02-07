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

struct book
{
	char author[20];
	int pages;
	float price;
};

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
	socklen_t clilen;
	struct sockaddr_storage cli_addr;
	struct addrinfo hints, *res;
	struct book b;

	memset(&hints,0,sizeof(hints));
	hints.ai_family =AF_UNSPEC;
	hints.ai_socktype =SOCK_DGRAM;

	if(getaddrinfo(argv[1],port,&hints,&res)==-1){ 
		error("Errot in addrinfo");
	}

	sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(sockfd < 0)
		error("Error in opening the socket");

	clilen = sizeof(cli_addr);

	if (sendto(sockfd, buffer, sizeof(buffer), 0, res->ai_addr, res->ai_addrlen) < 0) 
	{
		error("Error in sending ping to server");
	}

	if (recvfrom(sockfd,&b,sizeof(b),0,(struct sockaddr*)&cli_addr,&clilen) < 0)
	{
		error("Error in receiving from server");
	}

	printf("received a book from server\n");
	printf("author: %s\npages: %d\nprice: %f",b.author,b.pages,b.price);

	freeaddrinfo(res);
	close(sockfd);

	return 0;
}


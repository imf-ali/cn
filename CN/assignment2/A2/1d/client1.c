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

	printf("sending detail of a book to server\n");
	printf("Enter Author: ");
	scanf("%s",b.author);
	printf("Enter no. of Pages: ");
	scanf("%d",&b.pages);
	printf("Enter Price: ");
	scanf("%f",&b.price);

	if (sendto(sockfd, &b, sizeof(b), 0, res->ai_addr, res->ai_addrlen) < 0) 
	{
		error("Error in sending");
	}

	freeaddrinfo(res);
	printf("sent the book to %s\n",argv[1]);

	close(sockfd);

	return 0;
}


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
	int sockfd, newsockfd;
	char message[200];
	socklen_t clilen;
	struct sockaddr_storage cli_addr;
	struct addrinfo hints, *res;
	struct book b;

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

	if (recvfrom(sockfd,&b,sizeof(b),0,(struct sockaddr*)&cli_addr,&clilen) < 0)
	{
		error("Error in receiving from client1");
	}

	printf("received a book from client1\n");
	printf("author: %s\n",b.author);
	printf("pages: %d\n",b.pages);
	printf("price: %f\n",b.price);

	printf("now, change details of book\n");
	printf("Enter new Author: ");
	scanf("%s",b.author);
	printf("Enter no. of Pages: ");
	scanf("%d",&b.pages);
	printf("Enter Price: ");
	scanf("%f",&b.price);

	if (recvfrom(sockfd,message,sizeof(message),0,(struct sockaddr*)&cli_addr,&clilen) < 0)
	{
		error("Error in ping from client2");
	}

	if (sendto(sockfd, &b, sizeof(b), 0, (struct sockaddr*)&cli_addr, clilen) < 0) 
	{
		error("Error in sending to client2");
	}
	else
		printf("book sent to client2\n");
	
	

	freeaddrinfo(res);
	close(sockfd);
	

	return 0;
}

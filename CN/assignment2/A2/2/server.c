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

struct db
{
	int id;
	char name[20];
};

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

	FILE* fptr = fopen("data.txt", "r");
	int n; // no. of students in database
	int id; // id sent by client

	fscanf(fptr, "%d", &n);

	struct db student[n];

	for (int i = 0; i < n; ++i)
	{
		fscanf(fptr, "%d %s", &student[i].id, student[i].name); 
	}

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

	if (recv(newsockfd, &id, sizeof(id), 0) < 0)          //receive id from client
			error("Error in receiving id from client");

	int c = 1;
	for (int i = 0; i < n; ++i)
	{
		if (student[i].id == id)
		{
			if (send(newsockfd, student[i].name, sizeof(student[i].name), 0) < 0)          //send name corresponding to id to client back
				error("Error in sending student's name");
			c = 0;
			break;
		}
	}

	if (c)
	{
		char notFound[] = "id not found";
		if (send(newsockfd, notFound, sizeof(notFound), 0) < 0)          //send error message to client that id not found
				error("Error in sending 'id not found' error to client");
	}


	close(newsockfd);
	close(sockfd);
	

	return 0;
}

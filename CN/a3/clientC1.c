#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // for close
#include <string.h>
#include <netdb.h>

#define SERVERPORT "3491"
#define BACKLOG 10

int main(int v,char* argv[]){

	socklen_t addr_size;
	struct addrinfo hints,*res;
	int sockfd,new_fd;

	memset(&hints,0,sizeof(hints));
	hints.ai_family =AF_UNSPEC;
	hints.ai_socktype =SOCK_STREAM;
	hints.ai_flags=AI_PASSIVE;///

	if(getaddrinfo(NULL,SERVERPORT,&hints,&res)==-1){ ///
		perror("addrinfo");
		return 0;
	}

	if((sockfd=socket(res->ai_family, res->ai_socktype, res->ai_protocol))==-1){
		perror("sockfd");
		return 0;
	}
	
	if(connect(sockfd, res->ai_addr, res->ai_addrlen)==-1){
		perror("bind");
		return 0;
	}


	char msg[100];
	printf("Chat started print \"bye\" (Case sensitive) to exit \n write msg when prompt \"Your turn\"\n");

	while(1){
		int temp=(recv(sockfd,msg,sizeof(msg),0));
		if(temp==-1){
			perror("Send");
			break;
		}
		if(temp==0){
			printf("They exited\n");
			break;
		}
		printf("\nYour msg\n");
		fflush(stdout);
		printf("%s\n", msg);

		printf("Your turn\n");
		fflush(stdout);
		fgets(msg ,sizeof(msg),stdin);
		if((send(sockfd,msg,strlen(msg)+1,0))==-1)
			{
				perror("send");
				break;
			}
		if(strncmp("bye",msg,3)==0)
			break;

	}

	close(sockfd);
	return(0);
}

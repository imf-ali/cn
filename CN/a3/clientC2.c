#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // for close
#include <string.h>
#include <netdb.h>

#define SERVERPORT "3491"
#define BACKLOG 10

int main(int v,char* argv[]){

	int i;
	char msg[100];
	socklen_t addr_size;
	struct addrinfo hints,*res;
	int sockfd,new_fd;
	struct timeval tv;
	fd_set writefds,readfds,writefdsm,readfdsm;


	memset(&hints,0,sizeof(hints));
	hints.ai_family =AF_UNSPEC;
	hints.ai_socktype =SOCK_STREAM;


	if(getaddrinfo(argv[1],SERVERPORT,&hints,&res)==-1){ ///
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

	strcpy(msg,"start");
	if((send(sockfd,msg,strlen(msg)+1,0))==-1){
		perror("send");
		return 0;
	}

//////////////////////////////////////////////////////////////////////////////

	tv.tv_sec=0;
	tv.tv_usec=500000;
	FD_ZERO(&readfdsm);
	FD_SET(sockfd,&readfdsm);
	FD_SET(0,&readfdsm);

//////////////////////////////////////////////////////////////////////////////

	printf("Chat started print \"bye\" (Case sensitive) to exit\n");

	while(1){
		tv.tv_sec=0;
		tv.tv_usec=500000;
		readfds=readfdsm;


		if(select(sockfd+1,&readfds,NULL,NULL,NULL)==-1){
			perror("select");
			break;
		}

		for(i=0;i<=sockfd;++i){
			if(FD_ISSET(i,&readfds)){
				if(i==0){
					fgets(msg ,sizeof(msg),stdin);
					if((send(sockfd,msg,strlen(msg)+1,0))==-1){
							perror("send");
							break;
					}
					if(strncmp("bye",msg,3)==0)
						break;
				}

				else {
					int temp=(recv(i,msg,sizeof(msg),0));
					if(temp==-1){
						perror("recv");
						break;
					}
					if(temp==0){
						printf("They exited");
						break;
					}
					printf("\n==msg==\n");
					printf("%s\n", msg);
					fflush(stdout);
					if(strncmp("bye",msg,3)==0)
						break;
				}
			}
		}

		if(i<=sockfd)
			break;
	}

	close(sockfd);
	return(0);
}


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // for close
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

#define MYPORT "3491"
#define BACKLOG 10

void sigchld_handler(int s){    // waitpid() might overwrite errno, so we save and restore it:    
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);    
    errno = saved_errno;
}// get sockaddr, IPv4 or IPv6:


void *get_in_addr(struct sockaddr *sa){    
    if (sa->sa_family == AF_INET) {        
        return &(((struct sockaddr_in*)sa)->sin_addr);    
    }    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int max(int a,int b){
	if(a>b)return(a);
	else return(b);
}

int main(){

	int i;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	struct addrinfo hints,*res;
	int sockfd,new_fd;
	struct timeval tv;
	fd_set writefds,readfds,writefdsm,readfdsm;
    struct sigaction sa;    

	memset(&hints,0,sizeof(hints));
	hints.ai_family =AF_UNSPEC;
	hints.ai_socktype =SOCK_STREAM;
	hints.ai_flags= AI_PASSIVE; ///

	if(getaddrinfo(NULL,MYPORT,&hints,&res)==-1){///
		perror("addrinfo");
		return 0;
	}
	printf("Done addrinfo\n");
	

	if((sockfd=socket(res->ai_family, res->ai_socktype, res->ai_protocol))==-1){
		perror("sockfd");
		return 0;
	}
	printf("created the socket\n");

	if(bind(sockfd, res->ai_addr, res->ai_addrlen)==-1){
		perror("bind");
		return 0;
	}
	printf("bind is done\n");
	

	if(listen(sockfd,BACKLOG)==-1){
		perror("Listen");
		return 0;
	}
	printf("listening\n");

	addr_size=sizeof(their_addr);
	
	if((new_fd=accept(sockfd,(struct sockaddr*)&their_addr, &addr_size))==-1){
		perror("accept");
		return 0;
	}

	printf("accepted\n");
	char ip[sizeof(their_addr)];
	inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),ip,sizeof(their_addr));
	printf("connected with: %s\n", ip);

	int maxfd=max(sockfd,new_fd);

//////////////////////////////////////////////////////////////////////////////

	sa.sa_handler = sigchld_handler; // reap all dead processes    
    sigemptyset(&sa.sa_mask);    
    sa.sa_flags = SA_RESTART;    
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {        
        perror("sigaction");        
        exit(1); 
    }  

	tv.tv_sec=0;
	tv.tv_usec=500000;
	FD_ZERO(&readfdsm);
	FD_SET(new_fd,&readfdsm);
	FD_SET(sockfd,&readfdsm);
	FD_SET(0,&readfdsm);

//////////////////////////////////////////////////////////////////////////////


	char msg[100];
	printf("Chat started print \"bye\" (Case sensitive) to exit\n");

	while(1){
		tv.tv_sec=0;
		tv.tv_usec=500000;
		readfds=readfdsm;


		if(select(maxfd+1,&readfds,NULL,NULL,NULL)==-1){
			perror("select");
			break;
		}


		for(i=0;i<=maxfd;++i){
			if(FD_ISSET(i,&readfds)){
				if(i==0){

					fgets(msg ,sizeof(msg),stdin);
					if((send(new_fd,msg,strlen(msg)+1,0))==-1){
							perror("send");
							break;
					}
					if(strncmp("bye",msg,3)==0)
						break;
				}

				else if(i==sockfd){
					int new;
					if((new=accept(sockfd,(struct sockaddr*)&their_addr, &addr_size))==-1){
						perror("accept");
						continue;
					}
					printf("accepted\n");
					inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),ip,sizeof(their_addr));
					printf("connected with: %s\n", ip);

					if(fork()==0){
							FD_CLR(sockfd,&readfdsm);
							FD_CLR(new_fd,&readfdsm);
							//close(new_fd);
						    close(sockfd);// child doesn't need the listener
						    new_fd=new;
						    maxfd=new_fd;  
							FD_SET(new_fd,&readfdsm);
							readfds=readfdsm;
					}
					else close(new);  // parent doesn't need this
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

		if(i<=maxfd)
			break;
	}

	close(new_fd);
	close(sockfd);
	return(0);
}


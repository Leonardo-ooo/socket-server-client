#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
/*
*	struct sockaddr_in{
*		short int sin_family;				//		protocol type
*		unsigned short int sin_port;		// 		port number
*		struct in_addr sin_addr;			// 		ipv4 address
*		unsigned char sin_zero[8];			//		padding 
*	}
*/
int main(int argc, char* argv[]){

			//	set address and port
	
	char addr[16] = "192.168.213.128";
	short port = 8011;


			//	open a TCP socket (type: SOCK_STREAM)	

	int serverfd;

	if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("create socket error!\n");
		return -1;
	}


			//	set SO_REUSEADDR (solve address haven been used)

	int opt = 1; unsigned int len = sizeof opt;
	setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt, len);	


			// set server protocol address parameter
	
	struct sockaddr_in seraddr;
	memset(&seraddr, 0, sizeof seraddr);
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(port);
	//seradrr.sin_addr.s_addr = htonl(INADDR_ANY);
	seraddr.sin_addr.s_addr = inet_addr(addr);



			//	bind protocol address to the TCP socket
	
	if(bind(serverfd, (struct sockaddr *) &seraddr, sizeof(seraddr))){
		perror("bind error!\n");
		close(serverfd);
		return -1;
	}

	
			//	set listenning mod
	
	listen(serverfd, 5);

	
			//	waiting connect

	int clientfd;
	int socklen = sizeof (struct sockaddr_in);
	struct sockaddr_in cliaddr;
	clientfd = accept(serverfd, (struct sockaddr*) &clientfd, &socklen);
	
	printf("user : %s connected \n", inet_ntoa(cliaddr.sin_addr));

	
	close(clientfd); close(serverfd);	

	return 0;
}

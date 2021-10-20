#include <iostream>
#include <stdio.h>
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
#include <signal.h>

#include "TcpClient.h"
/*
*	struct sockaddr_in{
*		short int sin_family;				//		protocol type
*		unsigned short int sin_port;		// 		port number
*		struct in_addr sin_addr;			// 		ipv4 address
*		unsigned char sin_zero[8];			//		padding 
*	}
*/

using namespace std;

TcpClient *client = new TcpClient;

void EXIT(int sig)
{
    delete client;
    printf("client close !\n");
    exit(-1);
}

void signal_init()
{
	for(int i = 1; i <= 64; i ++) signal(i, SIG_IGN);
    struct sigaction sact;
    memset(&sact, 0, sizeof sact);
    sact.sa_handler = EXIT;
    sigaction(SIGINT, &sact, NULL);
    sigaction(SIGTERM, &sact, NULL);
}

void* receive(void* arg)
{
   pthread_detach(pthread_self());
   pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
   memset(client->recvbuffer, 0, sizeof client->recvbuffer);

   while(client->Read())
   {
        cout << "recv : " << client->recvbuffer << endl;
        memset(client->recvbuffer, 0, sizeof client->recvbuffer);
   }
   return 0;	
}

int main(int argc, char* argv[]){
    if(argc > 3)
    {
        cout << "argment wrong!" << endl;
        return -1;
    }

    unsigned short port = 8011;
    char host_name[30] = "106.86.80.37";
    
    if(argc >= 2)
    {
        port = (unsigned short)atoi(argv[1]);
        if(!port) {cout << "wrong port !" << endl; return -1;}
    }
    if(argc == 3)
    {
        memcpy(host_name, argv[2], sizeof argv[2]);
    }
    signal_init();
   
    if(client->Connect(host_name, port) == false)
    {
        cout << "connect failed !" << endl;
        delete client; 
        return -1;
    }
    cout <<"connected !" << endl;

    pthread_t pthid;
    pthread_create(&pthid, NULL, receive, NULL);

    while(true){
        cin >> client->sendbuffer;
        if(send(client->sockfd, client->sendbuffer, strlen(client->sendbuffer), 0) >= 0)
        {
            cout << " send " << client->sendbuffer << endl;   
            memset(client->sendbuffer, 0, sizeof client->sendbuffer);
        }
        else
        {
            pthread_cancel(pthid);
            break;
        }
    }
    
    delete client;

	return 0;
}

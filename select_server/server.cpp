#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <signal.h>
#include <vector>

#include "TcpServer.h"

using namespace std;

TcpServer *server = new TcpServer;

void *conn(void*);
void signal_init();
void broadcast(char*, fd_set*, int, int);
void EXIT(int);

int main(int argc, char *argv[])
{
    if(argc > 2)
    {
        cout << "argment wrong !" << endl;
        return -1;
    }

    unsigned short port = 8011;
    if(argc == 2) port = (unsigned short)atoi(argv[1]);
    if(!port){ cout << "wrong port!" << endl; return -1; }

    signal_init();
    server->InitServer(port);
    cout << "setver on ! listenfd = " << server->listenfd << endl;
    
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(server->listenfd, &rfds);
    int maxfd = server->listenfd;
    
    while(true)
    {
        fd_set tmps = rfds;

        //struct timeval sttime;
        //sttime.tv_sec = 5;

        int infds = select(maxfd + 1, &tmps, NULL, NULL, NULL);
        if(infds < 0)
        {
            cout << infds << " ";
            cout << "select function error! " << endl;
        }
        else if(!infds)
        {
            cout << "timeout !" << endl;
        }
		cout << "event number : " << infds << endl;
        for(int i = 0; i <= maxfd; i ++ )
        {
            if(!FD_ISSET(i, &tmps)) continue;
            if(server->listenfd == i)
            {
                server->Accept();
                FD_SET(server->clientfd, &rfds);
                maxfd = max(maxfd, server->clientfd);
				cout << server->GetIP() << " connect !" << endl;;
				cout << "maxfd = " << maxfd << endl;
            }
            else
            {
                if(recv(i, server->recvbuffer, 4096, 0) > 0)
				{
                    cout << "size = " << strlen(server->recvbuffer) << ", recv : " << server->recvbuffer << endl;
                	broadcast(server->recvbuffer, &rfds, maxfd, server->listenfd);
				}
				else
				{
					cout << "client fd = " << i << "closed !" << endl;
					close(i);
					FD_CLR(i, &rfds);

					if(i == maxfd)
					{
						for(int j = maxfd - 1; j > 2; j --)
							if(FD_ISSET(j, &rfds)) { maxfd = j; break; }
						cout << "maxfd change to " << maxfd << endl;
					}
				}

            }
        }

    }

    return 0;
}

void broadcast(char*  buffer, fd_set *rfds, int maxfd, int listenfd)
{
    for(int i= 1; i <= maxfd; i ++)
        if(FD_ISSET(i, rfds) && i != listenfd)
        {
            if(send(i, buffer, strlen(buffer), 0) <= 0)
                cout << "send to " << i << "failed! " << endl;
        }
    cout << "broadcast : " << buffer << endl;
    memset(buffer, 0, strlen(buffer));
}

void signal_init()
{
    for(int i = 1; i <= 64; i ++)signal(i, SIG_IGN);

    struct sigaction sact;
    memset(&sact, 0, sizeof sact);
    sact.sa_handler = EXIT;
    sigaction(SIGINT, &sact, NULL);
    sigaction(SIGTERM, &sact, NULL);
}

void EXIT(int sig)
{
    delete server;
    cout << "recv the " << sig << " signal exit!" << endl;
    exit(0);
}

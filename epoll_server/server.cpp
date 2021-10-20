#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <signal.h>
#include <vector>
#include <map>

#include "TcpServer.h"
#include "DBC.h"

#define SOCK_SIZE 1024

using namespace std;

TcpServer *server = new TcpServer;
map<int, int>sockmp;

void *conn(void*);
void signal_init();
void broadcast(char*);
void EXIT(int);

struct epoll_event& event(int fd, __uint32_t events)
{
    static epoll_event a;
    memset(&a, 0, sizeof a);
    a.data.fd = fd;
    a.events = events;

    return a;
}

int main(int argc, char *argv[])
{
    if(argc > 2)
    {
        cout << "argment wrong ! expected one argment port !" << endl;
        return -1;
    }

    unsigned short port = 8011;
    if(argc == 2) port = (unsigned short)atoi(argv[1]);
    if(!port){ cout << "wrong port!" << endl; return -1; }

    signal_init();
    server->InitServer(port);
    cout << "setver on ! listenfd = " << server->listenfd << endl;
    
    int epollfd;
    epollfd = epoll_create(1);
    struct epoll_event evts[SOCK_SIZE];
    struct epoll_event lis;
    lis.data.fd = server->listenfd;
    lis.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, server->listenfd, &event(server->listenfd, EPOLLIN));
    
    while(true)
    {
        int infds = epoll_wait(epollfd,evts, SOCK_SIZE, -1);
        if(infds < 0) { cerr << "epoll_wait() failed !\n"; return -1;}
        if(infds == 0)
        {
                //timeout
        }

        for(int i = 0; i < infds; i ++)
        {
            if(evts[i].data.fd == server->listenfd)
            {
                if(!server->Accept()) { cout << "accept error! \n"; continue; }
                cout << server->GetIP() << "  connect " << endl;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, server->clientfd, &event(server->clientfd, EPOLLIN));
                sockmp[server->clientfd] = -1;

            }
            else
            {
                if(recv(evts[i].data.fd, server->recvbuffer, 4096, 0) <= 0){
                    cout << evts[i].data.fd << " sock close" << endl;
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, evts[i].data.fd, &evts[i]);
                    sockmp.erase(sockmp.find(evts[i].data.fd));
                    continue;
                }
                cout << "recv : " << server->recvbuffer << endl;
                broadcast(server->recvbuffer);
            }
        }
                                        
    }

    return 0;
}

void broadcast(char*  buffer)
{
    for(map<int, int>::iterator i= sockmp.begin(); i != sockmp.end(); i ++)
    {
        send(i->first, buffer, strlen(buffer), 0);
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

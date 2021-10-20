#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
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
#include <iostream>

#define BUFFER_SIZE 4096

class TcpClient
{
public:
	int sockfd;
	struct sockaddr_in seraddr;

	TcpClient();
    ~TcpClient();

    bool Write();
    bool Read();

	bool Connect(const char *host, const unsigned short port);

    char *recvbuffer;
    char *sendbuffer;
};

#include "TcpClient.h"

TcpClient::TcpClient()
{
	sockfd = -1;
    recvbuffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);
    sendbuffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);
}


TcpClient::~TcpClient()
{
    close(sockfd);
}

bool TcpClient::Connect(const char *host, const unsigned short port)
{
	if(sockfd != -1) { close(sockfd); sockfd = -1;}


		//		create a client socket

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) return false;


		//		set server protocol address 
	
	struct hostent *host_addr = gethostbyname(host);
	if(!host_addr) return false;
	memset(&seraddr, 0, sizeof seraddr);
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(port);
	memcpy(&seraddr.sin_addr, host_addr->h_addr, host_addr->h_length);

		
		//		connect to the server
	if(connect(sockfd, (struct sockaddr*) &seraddr, sizeof seraddr))
	{
        
		close(sockfd);
		return false;
	}
	return true;
}

bool TcpClient::Write()
{
    if(send(sockfd, sendbuffer, strlen(sendbuffer), 0) <= 0) return false;
    memset(sendbuffer, 0, sizeof sendbuffer);
	return true;
}

bool TcpClient::Read()
{
	int a;
    if((a = recv(sockfd, recvbuffer, 4096, 0)) <= 0)
    {
    memcpy(recvbuffer, "pipe close !", 15);
    return false;
    }
	//std::cout << "recv size: " << a << std::endl;
    return true;
}

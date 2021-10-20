#makefile

#=======================================================================================================

#变量

main = client.cpp
obj = client.o TcpClient.o
app = client

#========================================================================================================

#目标

all:$(app)

$(app):$(obj)
	g++ $^ -o $@ -lpthread

client.o:$(main)
	g++ -c $< -I./lib/

TcpClient.o:./lib/TcpClient.cpp
	g++ -c $< -I./lib/

PHONY:clean
clean:
	-rm $(obj) $(app)
    


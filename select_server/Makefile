#Makefile

#=========================================================================================

#变量
VPATH = src include
main = server.cpp 
obj = server.o TcpServer.o
app = server

#=========================================================================================

#目标

all:$(app)

$(app):$(obj)
	g++  $^ -o $@ -lpthread

server.o:$(main) 
	g++ -c $< -I./lib/

TcpServer.o: ./lib/TcpServer.cpp
	g++ -c $< -I./lib/

.PHONY:clean

clean:
	-rm  $(obj) $(app)
    


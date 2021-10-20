#Makefile

#=========================================================================================

#变量
VPATH = src include
dbc = DBC.h
main = server.cpp 
obj = server.o TcpServer.o DBC.o
app = server

#=========================================================================================

#目标

all:$(app)

$(app):$(obj)
	g++  $^ -o $@ -lpthread -L /usr/lib/mysql -lmysqlclient

server.o:$(main) 
	g++ -c $< -I./lib/

TcpServer.o: ./lib/TcpServer.cpp
	g++ -c $< -I./lib/

DBC.o: ./lib/DBC.cpp
	g++ -c $< -I./lib/ -I/usr/include/mysql


.PHONY:clean

clean:
	-rm -rf  $(obj) $(app)
    


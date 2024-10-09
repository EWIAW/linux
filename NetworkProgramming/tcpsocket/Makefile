.PHONY:all
all:tcpclient tcpserver

tcpclient:tcpclient.cc
	g++ $^ -o $@ -std=c++11 
tcpserver:tcpserver.cc
	g++ $^ -o $@ -std=c++11 

.PHONY:clean
clean:
	rm -rf tcpclient tcpserver

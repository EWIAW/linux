.PHONY:all
all:UdpClient UdpServer

UdpClient:UdpClient.cc
	g++ $^ -o $@ -std=c++11 -lpthread
UdpServer:UdpServer.cc
	g++ $^ -o $@ -std=c++11

.PHONY:clean
clean:
	rm -rf UdpClient UdpServer
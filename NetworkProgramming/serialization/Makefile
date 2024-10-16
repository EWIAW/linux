.PHONY:all
all:calclient calserver

calclient:calclient.cc
	g++ $^ -o $@ -std=c++11 -ljsoncpp
calserver:calserver.cc
	g++ $^ -o $@ -std=c++11 -ljsoncpp

.PHONY:clean
clean:
	rm -rf calclient calserver

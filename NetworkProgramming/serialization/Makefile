.PHONY:all
all:calclient calserver

calclient:calclient.cc
	g++ $^ -o $@ -std=c++11 
calserver:calserver.cc
	g++ $^ -o $@ -std=c++11 

.PHONY:clean
clean:
	rm -rf calclient calserver

.PHONY:all
all:mysignal test

mysignal:mysignal.cc
	g++ $^ -o $@ -std=c++11 -g

test:test.cc
	g++ $^ -o $@ -std=c++11

.PHONY:clean
clean:
	rm -rf mysignal test
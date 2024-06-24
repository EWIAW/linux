.PHONY:
all:mybin myexec 

mybin:mybin.c
	gcc $^ -o $@
myexec:myexec.c
	gcc $^ -o $@ 
.PHONY:clean
clean:
	rm -rf myexec mybin 

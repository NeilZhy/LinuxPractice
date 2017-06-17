.PHONY:all 
all:myexec helloexec
myexec:myexec.c
	gcc -o $@ $^
helloexec:helloexec.c
	gcc -o $@ $^
.PHONY:clean
clean:
	rm -f myexec helloexec


.PHONY:all
all:read write
read:read.c
	gcc -o $@ $^
write:write.c
	gcc -o $@ $^
.PHONY:
clean:
	rm -f read write log.pipe

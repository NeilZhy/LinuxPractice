#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
int main()
{
	umask(0);
	int fd = open("./log",O_WRONLY|O_CREAT,0666);
//	close(1);
	fd = dup2(fd,1);
	printf("hahahahahahaha");
	return 0;
}

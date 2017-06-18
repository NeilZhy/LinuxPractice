#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define _PATH_ "/home/xiaoxu/code/fifo/log.pipe"

int main()
{
	umask(0);
	if(mkfifo(_PATH_,0666|S_IFIFO) < 0 )
	{
		printf("fifo error\n");
		return 1;
	}
	int fd = open(_PATH_,O_WRONLY);
	if(fd <= 0)
	{
		printf("open error\n");
		return 1;
	}
	else
	{
		char buf[100];
		memset(buf,'\0',sizeof(buf));
		while(1)
		{
			scanf("%s",&buf);
			int wr = write(fd,buf,sizeof(buf));
			if(wr <= 0)
			{
				printf("write error\n");
				break;
			}
		}

	}
	close(fd);
	return 0;
}

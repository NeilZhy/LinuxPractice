#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define _PATH_ "/home/xiaoxu/code/fifo/log.pipe"

int main()
{
	int fd = open(_PATH_,O_RDONLY);   //只读方式打开一个管道
	if(fd < 0)    //打开管道失败
	{
		printf("read error\n");
		return 1;
	}
	else   //打开管道成功
	{
		char buf[100];
		memset(buf,'\0',sizeof(buf));
		while(1)
		{
			int red = read(fd,buf,sizeof(buf));  //把管道里面的内容读取到buf中，所以上面应该加上memset这个函数，对buf进行初始化
			if(red <= 0)   //
			{
				printf("read error\n");
				break;
			}
			printf("%s\n",buf);
		}
		close(fd);
	}
	return 0;
}

#include<poll.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define MAX 64

int statup(char* ip,int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket error\n");
		exit(2);
	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);    //htons作用是将主机的数字序列转换成网络序列，这里因为使用的是AF_INET
									//占两个字节的大小，所以使用的是s
	local.sin_addr.s_addr = inet_addr(ip);   //inet_addr的作用是将点分十进制的字符创装换成网络序列的
	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
		perror("bind error\n");
		exit(3);
	}
	if(listen(sock,10)<0)
	{	
		perror("listen error\n");
		exit(4);
	}
	return sock;

}

void usage()
{
	printf("程序 + IP + 端口号\n");
}



int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		usage();
		exit(1);
	}
	int listensock = statup(argv[1],atoi(argv[2]));
	struct pollfd pollarr[64];	
	pollarr[0].fd = listensock;  //设置第一个为监听套接字
	pollarr[0].events = POLLIN;
	pollarr[0].revents = 0;
	int nfds = 1;   //用来记录结构体数组中，结构体的总个数，用于poll函数的第二个参数
	int i = 1;
	for(;i<MAX;i++)  //把数组的其他位置上面的数据置为无效的
	{
		pollarr[i].fd = -1;
		pollarr[i].events = 0;
		pollarr[i].revents = 0;
	}
	int count = 0;
	int timeout = 1000;
	while(1)
	{
		pollarr[0].fd = listensock;
		pollarr[0].events = POLLIN;
		pollarr[0].revents = 0;
		switch(poll(pollarr,nfds,1000))   //这个max可能是存在问题，可能的是我们的那个含有的最大的监听的个数
		{
			case -1:
				{
					printf("poll error\n");
					return -1;
				}
				break;
			case 0:
				{
					printf("timeout\n");
				}
				break;
			default:
				{
					int j = 0;
					for(;j<MAX;j++)
					{
						if((pollarr[j].revents&POLLIN )&&(pollarr[j].fd == listensock))   //监听套接字响应了,且是监听套接字
							//这里我一开始的时候，使用 的是event，这个是我要告诉系统的
							//但是应该使用的是revents
						{
							printf("监听成功了\n");
							struct sockaddr_in client;
							socklen_t len = sizeof(client);
							int newsock  = accept(listensock,(struct sockaddr*)&client,&len);
							if(newsock<0)
							{
								printf("accept error\n");
							}
							else
							{
								int m = 0;
								for(;m<MAX;m++)
								{
									if(pollarr[m].fd == -1)
									{
										printf("newsock %d\n",m);
										pollarr[m].fd = newsock;
										pollarr[m].events = POLLIN;
										pollarr[m].revents = 0;
										nfds++;
										break;   //这里需要退出循环，不然就肯定会到了下面的那个满了，然后那个newsock被关闭了
									}
									else
									{
										continue;
									}
								}
								if(m == MAX)
								{
									printf("满了\n");
									close(newsock);
								}
							}
	
						}
						if((pollarr[j].revents & POLLIN)&&(pollarr[j].fd != listensock))  //读写套接字
						{

							printf("有读写内容了\n");
							char buf[1024];
							int s = read(pollarr[j].fd,buf,sizeof(buf)-1);
							if(s > 0)
							{
								buf[s] = 0;
								printf("%s\n",buf);
							}
							else if(s == 0)   //对端关闭了
							
							{
								int w = 0;
								while(w++<10)
								{
								printf("对端关闭了\n");
								}
								close(pollarr[j].fd);
								pollarr[i].fd = -1;
								pollarr[i].events = 0;
								pollarr[i].revents = 0;
								--nfds;
							}
							else
							{
								printf("read error\n");
							}
						}
						else  //为了完整性，啥都不干
						{
							continue;
						}
					}
				}
				break;

		}
	}
	close(listensock);
    return 0;
}

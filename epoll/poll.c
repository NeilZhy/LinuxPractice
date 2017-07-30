#include<sys/epoll.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define SIZE 64

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
	printf("listensock\n");
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
	printf("%d\n",listensock);
	int epfd = epoll_create(SIZE);
    struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listensock;
   
	epoll_ctl(epfd,EPOLL_CTL_ADD,listensock,&ev);
	struct epoll_event ens[SIZE];
	int timeout = 2000;
	int  num = -1;
	while(1)
	{
		switch(num = epoll_wait(epfd,ens,SIZE,timeout))  //这里错误的原因是，我在ens前面加上了&，这个是错误
															//因为ens本来就是给 数组名，然后本来就是一个地址
		{
			case -1:
				{
					printf("epoll_wait\n");
				}
				break;
			case 0:
				{
					printf("time out\n");
				}
				break;
			default:
				{
					int i = 0;   //说明等待成功了 ，但是这个数组里面可能有三种 文件描述符，一种是listen文件描述符，一种是
								//读就绪文件描述符，一种是写就绪文件描述符，所以下面需要遍历这个数组
					for(;i<num;i++)
					{
						int fd = ens[i].data.fd;
						if(fd == listensock&&(ens[i].events&EPOLLIN))   //当前的文件描述符是listen的，而且当前的文件是输入的
						{
							struct sockaddr_in client;
							int len = sizeof(client);
							int newsock = accept(fd,(struct sockaddr*)&client,&len);
                         	ev.events = EPOLLIN;
	                        ev.data.fd = newsock;
                         	epoll_ctl(epfd,EPOLL_CTL_ADD,newsock,&ev);
						}
						else if(fd != listensock)  //当前的文件描述符是读的或者是写的
						{
							if(ens[i].events&EPOLLIN)  //读事件就绪了
							{
								char buf[1024];
								int s = read(fd,buf,sizeof(buf)-1);
								if(s>0)  //读数据成功了
								{
	                    	        buf[s] = 0;
									printf("%s\n",buf);
                             		ev.events = EPOLLOUT;
	                           	 	ev.data.fd = fd;
                         	    	epoll_ctl(epfd,EPOLL_CTL_MOD,ens[i].data.fd,&ev);
								}
								else if(s == 0) //说明对端关闭了，这个就是死记就好了，就是这么规定的
								{
									close(ens[i].data.fd);
									epoll_ctl(epfd,EPOLL_CTL_DEL,ens[i].data.fd,NULL);
								}
								else  //读失败了
								{
									close(ens[i].data.fd);
									epoll_ctl(epfd,EPOLL_CTL_DEL,ens[i].data.fd,NULL);
								}
							}
							else if(ens[i].events&EPOLLOUT)  //写事件就绪
							{
								//写一个就关闭他
								char *msg = "HTTP/1.0.200 OK\r\n\r<html><h2>l love baixu</h2></html>/r/n";
								write(ens[i].data.fd,msg,strlen(msg)-1);
								close(ens[i].data.fd);
								epoll_ctl(epfd,EPOLL_CTL_DEL,ens[i].data.fd,NULL);
							}
							else  //其他事件，也不关心了
							{}
						}
						else   //为了配合if的完整性，不用管他
						{}
					}
				}
				break;
		}
	}


	return 0;

}

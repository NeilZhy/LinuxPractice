#include<stdio.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<stdlib.h>
#define size 64

const char* msg = "HTTP/1.0 200 OK\r\n<html><h1>hahahha</h1></html>";

void usage(const char *proc)
{
	printf("Usage : %s [local_ip] [local_port\n]",proc);
}


int startup(const char *ip,int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(3);
	}

	int opt =1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	//fuquqi tuichu zhijie chongqi 


	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
	socklen_t len = sizeof(local);

	if(bind(sock,(struct sockaddr*)&local,len) < 0)
	{
		perror("bind");
		exit(4);
	}
	if(listen(sock,10) < 0)
	{
		perror("listen");
		exit(5);
	}
	return sock;
}

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		usage(argv[0]);
		return 1;
	}

	int listen_sock = startup(argv[1],atoi(argv[2]));
	printf("listen_sock success  %d\n",listen_sock);
	int epfd = epoll_create(256);
	if(epfd < 0)
	{
		perror("epoll create");
		return 2;
	}

	printf("sock : %d, epfd : %d\n",listen_sock,epfd);

	//监听套接字，就是我们的listen_sock，是需要关心底层数据的，就是这里的，
	//比如我们的底层是有链接来了的，这里其实也就是我们的底层有数据的改变的
	//所以我们的listen_sock也需要关心我哦们的底层的数据的
	
	//所以我们这里是不能直接accept的，因为如果我哦们这里直接就是accept的话
	//我们这里的accept函数就直接阻塞在哪里了
	
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;

	printf("list_sock1  %d\n",listen_sock);
	

	//我们这里使用的是一个结构体，这个内容实际上就是我们的epoll那个红黑树
	//里面的内容，这个时候，一个结构体标识的就是一个事件，我们是需要关心
	//的就是我们的listen_sock的是否有输入事件发生了
	//同时这里的那个data.fd我不知到是什么意思
	//通过查看资料我发现，这个data.fd实际上保存我们的关系的某个文件描述符
	//就是我们这里使用了一个struct结构体，但是这个结构体关心的是哪个文件描述
	//我们这里是需要写上的
	int efd = epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&ev);
	if(efd == 0)
	printf("epoll_ctl success\n");
	// 这个时候，我们的那个就注册进去了
	int nums = -1;
	int timeout = -1;
	struct epoll_event revs[size];
//这里如果是就绪了，就被写在了revs里面了
//zhe li revs xiangdnegyu shi nage duilie 
	while(1)
	{
		printf("while\n");
		switch(( nums = epoll_wait(epfd,revs,size,timeout))) //如果事件就绪了
			//事件就被写在了revs里面了
		{
			case -1:
			perror("wait ");
			break;
			case 0:
			printf("timeout .... \n");
			break;
			default:
			{
			    printf("%d\n",nums);
				printf("wati epoll io\n");
				//成功了
				int i = 0;
				for(;i < nums;i++)
				{
					int fd = revs[i].data.fd;
					if(fd == listen_sock && (revs[i].events & EPOLLIN))
					{
						
						//listen——sock就绪了
					
						//
						struct sockaddr_in client;
						socklen_t len = sizeof(client);
						int rw_sock = accept(listen_sock,(struct sockaddr*)&client,&len);
						if(rw_sock < 0)
						{
						
							perror("accept");
				

						
							continue;
						}
						printf("get a new client [%s : %d]\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
						ev.events = EPOLLIN;
						ev.data.fd = rw_sock;
						epoll_ctl(epfd,EPOLL_CTL_ADD,rw_sock,&ev);

					
						printf("new sock\n");
					}
					else if(fd != listen_sock)
					{
						//说明不是监听套接字，那么就是其他的文件描述符
						//那么这个时候的其他事件，可能就是其他的事件，我们
						//这个时候需要判断我们的事件是读事件还是写事件
						//就绪了呢

						printf("IO success\n");
						if(revs[i].events & EPOLLIN)
						{
							
							//读事件就绪了
							char buf[1024] = {0};
							ssize_t s = read(fd,buf,sizeof(buf)-1);
							if(s > 0)
							{
								buf[s] = 0;
                                printf("client ## %s\n",buf);
								//du chenggong zai guanxin xie 
								ev.events = EPOLLOUT;
								ev.data.fd = fd;
							
								
							
								epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev);
								
								write(fd,msg,sizeof(msg));
							}
							else if(s == 0)
							{
								
								printf("client is quit\n");
								close(fd);
								
								
								
								epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
							}
							else  //s < 0
							{
								perror("read");
								close(fd);
								
							
							
								epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
						
						
						
							}

						}
						else if(revs[i].events & EPOLLOUT)
						{
							//写事件就绪了
							write(fd,msg,sizeof(msg));
							close(fd);
							epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);

						}
						else
						{
							//其他的情况
						}



					
					}
					else
					{
						//其他的内容了
	
				

					}
				}
			}
		}
	}
	close(listen_sock);
	close(epfd);
	return 0;
}

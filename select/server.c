#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<time.h>
#include<sys/select.h>
#include<stdlib.h>
#define SIZE 64

void usge()
{
	printf("程序 + IP地址 + 端口号\n");
	return;
}

int startup(char *ip,int port)   //创建一个监听套接字
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		printf("socket error\n");
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port); 
	local.sin_addr.s_addr = inet_addr(ip); 
	
	int fbind = bind(sock,(struct sockaddr*)&local,sizeof(local));	
	if(fbind<0)
	{
		printf("bind error\n");
	}
	int listensock = listen(sock,10);
	if(listensock<0)
	{
		printf("listen error\n");
	}
	//return listensock; 一开始我这里写错了，不应该是listensock，listensock只是给标识，就是看我是否监听成功了，所以这是命名的不规范
	return sock;

}


int fds[SIZE];

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		usge();
	}

	int listensock = startup(argv[1],atoi(argv[2]));
	if(listensock > 0)
	{
		printf("creat listen success \n");
	}

	int i = 0;
	for(;i<64;i++)
	{
		fds[i] = -1;
	}

	while(1)
	{
		int max = -1;
		fds[0] = listensock;
		fd_set set;
		FD_ZERO(&set);
		FD_SET(listensock,&set);
		int m = 0;	
		struct timeval timeout = {1,0};
		int s = 0;
		for(;s<SIZE;s++)
		{
			if(fds[s] != -1)
			{
				if(fds[s]>max)
				{
					max = fds[s];
				}
				FD_SET(fds[s],&set);  
			}
		
		}
		switch(select(max+1,&set,NULL,NULL,&timeout))
		{
			case -1:
				{
					printf("select error\n");
					break;
				}
				break;
			case 0:
				{
					printf("wait timeout\n");
					continue;
				}
				break;
			default:
				{	
					int a = 0;
					for(;a<SIZE;s++)
					{
						if(fds[a] == -1)
						{
							printf("lllllll\n");
							break;//continue;   //这里应该是continue还是应该是break；
						}
						else if((FD_ISSET(fds[a],&set))&&(fds[a] == listensock))
						{
							struct sockaddr_in client;
							socklen_t len = sizeof(client);

							int newsock  = accept(listensock,(struct sockaddr*)&client,&len);
							int b = 0;
							for(;b<SIZE;b++)
							{
								if(fds[b] == -1)
								{
									fds[b] = newsock;
									FD_SET(newsock,&set);
									break;
								}
							}
						}
						else if(FD_ISSET(fds[a],&set))
						{
							char buf[1024];
							ssize_t r = read(fds[a],buf,sizeof(buf)-1);
							if(r>0)
							{
								buf[r] = 0;
								printf("%s\n",buf);
							}
							if(r == 0)    //这里一句没有写,这一层判断没有写





							{
								printf("client closed\n");
								close(fds[a]);
								fds[a] = -1;
								continue;
							}
						}
						else 
						{
							break;
						}
					}
				}
				break;
		}
	}
	close(listensock);
	return 0;
}


//defluat:
  //可能 是无效的，可能是listen的变化，可能是是newsock的变化	
			   	//进入到default这个阶段的话，说明我们的集里面是有状态的变化的，但是不知道是listensock的变化
				//还是newsock的变化，同时我们还应该注意是把newsock加入到数组和集里面去
				//同时还应该注意的是，如果是new的变化的话，则应该打印了


//	int newsock = accept(listensock,(struct sockaddr*)&client,&len);
//	if(newsock > 0)
//	{
//		printf("链接成功\n");
//	}
//	int id = fork();
//	while(1)
//	{
//		struct timeval time = {5,0};
//		fd_set set;
//		FD_ZERO(&set);
//		FD_SET(newsock,&set);
//		int sel = select(newsock+1,&set,NULL,NULL,&time);
//		if(sel > 0)
//		{
//			char buf[1024];
//			read(newsock,buf,sizeof(buf)-1);
//			printf("%s",buf);
//		}
//		if(sel == 0)
//		{
//			printf("等待超时\n");
//		}
//	}

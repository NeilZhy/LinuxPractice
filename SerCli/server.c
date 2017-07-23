#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int stat(char* ip,int port)  //创建一个 listen套接字
{
	int sockid = socket(AF_INET,SOCK_STREAM,0);
	if(sockid<0)  //调用函数，创建一个套接字
	{
		perror("socket");
//		exit(1);
	}
	else
	{
		printf("socket success\n");
	}
	//套接字创建成功
	
	//创建ip地址结构体
	//上面使用的是IPv4的，所以这里创建的结构体是
    struct sockaddr_in local;   //
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);

	//绑定端口号
	if((bind(sockid,(struct sockaddr*)&local,sizeof(local)))<0)  //注意第二个参数要强转的，转成统一的ip
	{
		perror("bind");
//		exit(2);
	}
	else 
	{
		printf("bind success\n");
	}
	
	//设置监听结构体
	if(listen(sockid,10)<0)
	{
		perror("listen");
//		exit(3);
	}
	else
	{
		printf("listen success \n");
	}
	return sockid;
}

void usge()
{
	printf("输入ip和端口号");
}

int main(int argc,char* argv[])  //因为我们在执行这个程序的时候，需要填写ip和端口号，所以这里设置应该是可变参数的
{
	if(argc != 3)
	{
		usge();
	}

	//使用上面的函数创建一个监听套接字
	int listen_sock = stat(argv[1],atoi(argv[2]));  //这里的listen_sock类似于一个文件描述符
	printf("%d\n",listen_sock);
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	//接客
	while(1)  //不断的去接客
	{
		printf("接客\n");
		int newsock;
		newsock = accept(listen_sock,(struct sockaddr*)&client,&len);
		if(newsock<0)
		{
			printf("newsock error\n");
			//perror("newsock");
			continue;
		}
		else
		{
			printf("newsock success\n");
		}
		char buf[1024];
		while(1)
		{
			int size;
			size = read(newsock,buf,sizeof(buf)-1);
			if(size>0)
			{
			buf[size] = '\0';
			printf("client  say##  %s\n",buf);
			}
			if(size==0)
				break;
			printf("server enter##\n");
			size_t s = read(0,buf,sizeof(buf)-1);
			if(s>0)
			{
				buf[s-1] = 0;
				write(newsock,buf,sizeof(buf)-1);
			}
		}


	}

	
	return 0;
}

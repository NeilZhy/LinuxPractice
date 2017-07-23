#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argc,char* argv[])  //创建一个 listen套接字
{
	int sockid = socket(AF_INET,SOCK_STREAM,0);
	if(sockid<0)  //调用函数，创建一个套接字
	{
		perror("socket");
//		exit(1);
	}
	//套接字创建成功
	
	//创建ip地址结构体
	//上面使用的是IPv4的，所以这里创建的结构体是
    struct sockaddr_in local;   //这个local最好起名字是server，它是server的 结构体
	local.sin_family = AF_INET;
	local.sin_port = htons(atoi(argv[2]));
	local.sin_addr.s_addr = inet_addr(argv[1]);
	if(connect(sockid,(struct sockaddr*)&local,sizeof(local))<0)
	{
		perror("connet");
	}
	char buf[1024];
	while(1)
	{
	//	int size = read(sockid,buf,sizeof(buf)-1);
	//	if(size>0)
	//	{
	//		buf[size] = '\0';
	//		printf("%s\n",buf);
	//	}

		printf("client enter##\n");
		fflush(stdout);
	//	scanf("%s");
		size_t s = read(0,buf,sizeof(buf)-1);
		if(s>0)
		{
			buf[s-1] = 0;
			write(sockid,buf,strlen(buf));
		//	ssize_t _s = read(sockid,buf,sizeof(buf)-1);
		//	if(_s>0)
		//	{
		//		buf[s] = 0;
		//		printf("%s",buf);
		//	}
		}
	
	}
	return 0;


	
}



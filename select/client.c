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
	}
	
    struct sockaddr_in local;   //这个local最好起名字是server，它是server的 结构体
	local.sin_family = AF_INET;
	local.sin_port = htons(atoi(argv[2]));
	local.sin_addr.s_addr = inet_addr(argv[1]);
	if(connect(sockid,(struct sockaddr*)&local,sizeof(local))<0)
	{
		printf("connect error");
	}
	else
	{
		printf("链接成功了\n");
	}
	char buf[1024];
	while(1)
	{
		scanf("%s",&buf);
		write(sockid,buf,strlen(buf));
	}
	return 0;
}



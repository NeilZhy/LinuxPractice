#include"http.h"


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
	int on=1;  //允许重新绑定
	if((setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
	{  
		perror("setsockopt failed");  
		exit(EXIT_FAILURE);  
    } 

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



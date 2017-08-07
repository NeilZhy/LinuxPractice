#include"http.h"
#include<stdio.h>

void usge()  //用户函数
{
	printf("程序 + IP地址 + 端口号\n");
	return;
}

static void *accept_request(void* arg)
{
	int sock = (int)arg;
	pthread_detach(pthread_self());

	printf("我是一个线程%d\n",sock);




	//第三步就是编写这个handler_sock这个函数了
	return (void *)handler_sock(sock);   //这里是又调用了一个函数，函数参数是一个sock，这个sock就是为了处理获得的链接的

}

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		usge();
	}




	//第一步：创建一个监听套接字
	//因为我们这里是服务器，要监听来自网络的链接请求，我们的服务器是基于TCP/IP的
	int listensock = startup(argv[1],atoi(argv[2]));
	if(listensock > 0)
	{
		printf("creat listen success \n");
	}




	//第二步：不断的获取连接，然后创建线程
	//线程调用线程函数，去处理各个链接强求
	while(1)
	{
		struct sockaddr_in peer;
		socklen_t len = sizeof(peer);
		int sock = accept(listensock,(struct sockaddr*)&peer,&len);
		if(sock<0)
		{
			printf("accept error\n");
			continue;   //这里使用continue，因为如果woo这次即是获取一个链接失败了，我下次还客户继续循环获取就可以了
		}
		//获取连接成功了之后，就创建线程去处理它
		pthread_t tid;
		//调用线程函数去处理这个链接
		pthread_create(&tid,NULL,accept_request,(void *)sock);
		sleep(6);
		pthread_exit(NULL);
	}

	return 0;

}

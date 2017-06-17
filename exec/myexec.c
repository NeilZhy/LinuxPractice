#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	pid_t son = vfork();
	if(son == 0)  //子进程
	{
		printf("i am son. my id is %d\n",getpid());
		execl("/bin/ls","ls","-a","-n","-l",NULL);
		printf("调用exec函数失败\n");
		exit(0);
	}
	else //父进程
	{
		printf("i am father\n");
		sleep(5);
	}
	
	return 0;
}

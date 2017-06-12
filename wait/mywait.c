#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
int main()
{
	pid_t sunpid = fork();
	if(sunpid < 0)
	{
		printf("fork error\n");
	}
	else if(sunpid == 0)   //子进程
	{
		printf("i am sun\n");
		//sleep(10);
		printf("i will exit. my pid is %d\n",getpid());
		sleep(10);
		exit(9);  //我刚刚测试这个代码的时候，一直是等待失败，可能 和我这个exit有关，我不用这个了，直接什么都不用，看看是什么效果
	}
	else  //父进程
	{
		int status;
		pid_t sun;
		//这里调用的是阻塞式等待，所以预期的结果是子进程打印出了i am sun之后五秒中之后，子进程打印i will exit ...然后父进程打印子进程的id，等待成功
		while(1)
		{
			sleep(10);
		sun = wait(&status);  //如果等待成功了返回等待进程的pid，如果失败则返回-1
		if(sun == -1)
		{
			printf("等待失败\n");
			sleep(1);
		}
		else
		{
			printf("等待成功,子进程id是%d\n",sun);
			printf("返回码为%d\n",WEXITSTATUS(status));
			printf("返回状态为%d\n",WIFEXITED(status));
			break;
		}
		}
	}
	return 0;
}

#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
int main()
{
	int sv[2];
	int pair = socketpair(AF_LOCAL,SOCK_STREAM,0,sv);
	pid_t pid = fork();
	 if(pid == 0) 	//子进程
	{
		char buf[1024];
		int i = 0;
		char *hello = "hello";
		char *helloworld = "helloworld";
		char *msg = "hello";
		while(i++<10)
		{
			close(sv[0]);
			write(sv[1],hello,strlen(hello));
			write(sv[1],helloworld,strlen(helloworld));
		}
		close(sv[1]);
		open(sv[0]);
		    read(sv[0],buf,sizeof(buf)-1);
			printf("%s",buf);
			close(sv[0]);
			open(sv[1]);
		    write(sv[1],hello,strlen(hello));
			printf("%s\n",buf);
		printf("子进程退出\n");
		return 0;
	
	}
	else 	//父进程
	{
		char buf[1024];
		sleep(10);
		int status;
		
		close(sv[1]);
		read(sv[0],buf,sizeof(buf)-1);
		wait(&status);
		printf("%s\n",buf);
	}
	close(sv[0]);
	close(sv[1]);
	return 0;
}

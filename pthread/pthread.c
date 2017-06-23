#include<stdio.h>
#include<pthread.h>

void* pthread_run(void* arg)   //返回值void*，参数void*
{
	pthread_cancel(pthread_self());
	printf("进程id  ：%d   线程id：%u\n",getpid(),pthread_self());
	printf("i am thread!\n");
	sleep(3);
	//return (void*)1;    //返回值void*
	pthread_exit((void*)123);  //这里的返回作用和上面的return的效果是一样的
}


int main()
{
	pthread_t pid;   //定义一个线程地址变量，供pthread_create使用
	int ret = pthread_create(&pid,NULL,pthread_run,NULL);   //&pid：当pthread_create执行之后会把pid赋值，NULL线程属性，pthread_run线程函数，NULL：函数参数
	//pthread_cancel(pid);
	sleep(5);
	void* join;
	pthread_join(pid,&join);
	printf("进程等待%d\n",join);
	printf("进程id  ：%d  线程id ：%u\n",getpid(),pthread_self());
	printf("%d\n",ret);
	return 0;
}

#include<stdio.h>
#include<pthread.h>
int count = 0; 

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* pccount(void* arg)
{
	int i = 0;
	int temp = 0;
	while(i<50000000)
	{
		pthread_mutex_lock(&lock); 
	//	temp=count;
		count=count+1;
		//printf("%d",temp);
		i++;
		pthread_mutex_unlock(&lock); 
	}
//	printf("%d\n",count);
	pthread_exit((void*)123);
}

int main()
{
	pthread_t pid1;
	pthread_t pid2;
	int i = pthread_create(&pid1,NULL,pccount,NULL);

	int m = pthread_create(&pid2,NULL,pccount,NULL);
	void* join1;
	void* join2;
	pthread_join(pid1,&join1);

	pthread_join(pid2,&join2);  //这里必须要有线程等待，不然没有打印结果
    pthread_mutex_destroy(&lock); 
	printf("%d\n",count);
	return 0;

}

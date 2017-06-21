#include"comm.h"


int main()
{
	int i = 0;
	int shmid = creatShm(27);
	char* arr = (char*)shmat(shmid,NULL,0);   //这里可以把shmat当成是malloc使用，shmid:信号量标识符
										//NULL：const void* shmaddr，虚拟地址空间，设置为NULL，这个是系统自己定义的
										//返回值void* 这个时候我们要强制转换成我们需要的类型
	for(;i<26;)
	{
		sleep(1);
		arr[i] = 'A' + i;   //这里可以直接当成是malloc开辟的一个数组使用
		i++;
		arr[i] = 0;
	}
	int dt =  shmdt(arr);   //arr:这个就是我们上面的那个内存空间地址
	if(dt < 0)
	{
		perror("shmdt\n");
		return -1;
	}
	desShm(shmid);
	return 0;
}

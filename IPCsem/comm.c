#include"comm.h"

int commSem(int semnum,int flag)  //semnum是创建的信号量集的个数，flag是标志位，确定是创建新的
	     //还是获取一个已经创建的信号量集
{
	key_t key = ftok(PATH_NAME,POS_ID);   //第一个参数是路径，在头文件中已经定义了，是"."
										//第二个参数是标志位
	int semid = semget(key,semnum,flag);  //第一个是生成的key值，第二个参数是生成信号量集的个数
											//第三个参数是操作符标志位，就是本次操作是获取信号量还是创建信号量
											//返回值，如果失败返回-1，如果成功返回的是信号量标识符（信号量ID）
	if(semid < 0)
	{
		printf("commSem error\n");
		return -1;
	}
	return semid;
}


int creatSem(int semnum)
{
	return commSem(semnum,IPC_CREAT|IPC_EXCL|0666);
}


int desSem(int semid)
{
	semctl(semid,0,IPC_RMID);
}


int getSem(int semnum)
{
	return commSem(semnum,IPC_CREAT);
}

int P()
{}
int V()
{}


		

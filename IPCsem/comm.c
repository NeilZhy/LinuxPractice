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

void initSem(int semid,int which,int val)
{
	//这里解释一下上面参数的作用，第一个是我们要操作的信号量id，第二个是信号量集中的第几个信号量，因为信号量集中 有好多个信号量，第三个是初始化信号量的值是多少
    union semnu _semnu;    //这里在头文件中定义了一个结构
	//体，这个结构体是用来做初始化用的，如果我们此时的semctl中的第三个参数是IPC_EXCLRMID则是删除信号量，但是此时的是SETVAL，表示初始化信号量，这个时候才会用到这个结构体，结构体在头文件中定义了
	_semnu.val = val;  //这个结构体的作用就是设置信号量初始值
	if(semctl(semid,which,SETVAL,_semnu) < 0)
	{
		printf("initSem error\n");
		return;
	}
	return;
}

int desSem(int semid)
{
	semctl(semid,0,IPC_RMID);
}


int getSem(int semnum)
{
	return commSem(semnum,IPC_CREAT);
}


static int pv(int semid,int num,int flag)  //参数的含义是第一个是要告诉是哪个信号量集，第二个参数信号量 集中的第几个
	//第三个参数是表示进行 的是哪一种操作，是P还是V
{
	struct sembuf _sf;  //这个结构体是系统自己定义的，我们可以直接使用
	_sf.sem_num = num;
	_sf.sem_op = flag;
	_sf.sem_flg = 0;
	if(semop(semid,&_sf,1)<0)  //这里的semid是信号量集的id，&_sf是规定 的一个结构体，通过结构体进行操作，1，是结构体的个数
	{
		printf("PVerror\n");
		return -1;
	}
	return 0;
}
int P(int semid,int num)
{
	return pv(semid,num,-1);
}
int V(int semid,int num)
{
	return pv(semid,num,1);
}


		

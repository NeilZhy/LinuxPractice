#ifndef _COM_H_
#define _COM_H_

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

#define PATH_NAME "."
#define POS_ID 0x6666

union semnu   //初始化结构体，这个结构体是semctl这个函数的
//的第三个参数是SETVAL的时候使用的，这个结构体需要自己定义
{
	int val;
	struct semid_ds *bf;
	unsigned short* arry;
	struct seminfo* _buf;
};

//struct sembuf
//{
//	unsigned short sem_num;  //信号量集中的第几个
//	short sem_op;   //-1表示p操作，1表示v操作
//	short sem_flg;   //这里可以先设置0
//};

int commSem(int sennum,int flag);
int creatSem(int sennum);
void initSem(int semid,int which,int val);
int desSem(int semid);
int getSem();
int P(int semid,int num);
int V(int  semid,int num);





#endif

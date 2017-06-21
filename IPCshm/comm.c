#include"comm.h"

static int commShm(int size,int flag)   //size:共享内存的大小，flag：进行何种操作，是获取还是创建
{
	key_t key = ftok(PATHNAME,ID);
	if(key < 0)
	{
		perror("key_t\n");
		return -1;
	}
	int shmid = shmget(key,size,flag);  //key:键值，size：大小，这里传入的大小最好是4k（4096）的倍数
									//因为4096是一页，如果我们传递的是4097，系统会自动的对齐到8192，
									//但是因为我们申请的是4097，剩下的内存系统不会给你，flag：创建方式
	if(shmid < 0)
	{
		perror("shmget\n");
		return -2;
	}
	return shmid;
}

int creatShm(int size) //size：创建内存空间的大小
{
	return commShm(size,IPC_CREAT|IPC_EXCL|0666);
}


int getShm(int size)
{
	return commShm(size,IPC_CREAT);   //size：默认设置为0
}


int desShm(int shmid)
{
	int des = shmctl(shmid,IPC_RMID,NULL);	  //shmid:删除的是哪一个共享内存，IPC_RMID：控制方式，这里是删除的意思
											//NULL：这里是struct shmid_ds *buf，这是一个维护共享 内存的一个结构体
											//暂时不关心
	if(des < 0)
	{
		perror("shmctl\n");
		return -1;
	}
	return des;
}




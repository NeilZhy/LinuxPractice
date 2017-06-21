#ifndef _COMM_H_
#define _COMM_H_


#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define PATHNAME "."
#define ID 0x6666

int creatShm(int size);
int getShm(int size);
int desShm(int shmid);



#endif

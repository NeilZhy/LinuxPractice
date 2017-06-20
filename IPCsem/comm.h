#ifndef _COM_H_
#define _COM_H_

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

#define PATH_NAME "."
#define POS_ID 0x6666

int commSem(int sennum,int flag);
int creatSem(int sennum);
int desSem(int semid);
int getSem();
int P();
int V();





#endif

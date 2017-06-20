#ifndef __COMM_H__
#define __COMM_H__

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<string.h>

typedef struct _msginfo{
	long mtype;
	char mtext[1024];
}msginfo;

#define CLIENT 1
#define SERVER 2

int getMsgQueue();
int creatMsgQueue();
int commMsgQueue(int flag);
int desMsgQueue(int id);
int commMsg();
int sndMsg(int id,int who,char* sndbuf);
int rcvMsg(int id,int who,char* recbuf);


#endif

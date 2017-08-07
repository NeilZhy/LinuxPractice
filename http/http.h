#ifndef __HTTP_H__
#define __HTTP_H__



#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<time.h>
#include<sys/select.h>
#include<stdlib.h>
#include<pthread.h>


#define SIZE 4096
#define METHOD_SIZE 64
#define URL_SIZE SIZE


int startup(char *ip,int port);   
int handler_sock(int sock);




#endif




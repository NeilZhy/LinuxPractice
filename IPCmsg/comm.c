#include"comm.h"

int creatMsgQueue()
{
	key_t key = ftok(".",0x6666);   //第一个参数是路径名，第二个参数是随意给的一个,这里我如果使用的是"./queue"就错误了
	if(key < 0)
	{
		printf("ftok error\n");
		return -1;
	}
	int queue = msgget(key,IPC_CREAT|IPC_EXCL|0666);   //key是生成的一个端口，后面的两个宏时固有的参数，0666是访问 的权限
	if(queue < 0)
	{
		printf("msgget error\n");
		return -2;
	}
}

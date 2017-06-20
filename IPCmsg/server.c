#include"comm.h"
int main()
{
	int id = creatMsgQueue();
	char  sndbuf[1024];
	char recbuf[1024];
	int i = 0;	
	printf("%d\n",id);
//	printf("creat queue success\n");
	while(i++ < 10)
	{
		printf("请输入：");
		scanf("%s",sndbuf);
	//	sleep(1);
	//	rcvMsg(id,CLIENT,recbuf);
	//	printf("%s",recbuf);
		//sleep(1);		
		sndMsg(id,SERVER,sndbuf);
		rcvMsg(id,CLIENT,recbuf);
		printf("client say # %s\n",recbuf);
	}
	desMsgQueue(id);
	return 0;
}

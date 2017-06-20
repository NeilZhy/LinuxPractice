#include"comm.h"
int main()
{
//	int id = creatMsgQueue();
	char  sndbuf[1024];
	char recbuf[1024];
	int i = 0;
	int id = getMsgQueue();	
//	printf("creat queue success\n");
	while(i++ < 10)
	{
		//printf("请输入：");
		//scanf("%s",sndbuf);
	//	sleep(1);
	//	rcvMsg(id,CLIENT,recbuf);
	//	printf("%s",recbuf);
		//sleep(1);		
		//sndMsg(id,CLIENT,sndbuf);
        rcvMsg(id,SERVER,recbuf);
		printf("server say # %s\n",recbuf);
		printf("请输入 #");
		scanf("%s",sndbuf);
		//sleep(1);
		sndMsg(id,CLIENT,sndbuf);
	}
	//desMsgQueue(id);
	return 0;
}

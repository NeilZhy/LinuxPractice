#include"comm.h"

int commMsgQueue(int flag)   //这里封装了一个关于消息队列的一个函数，通过传入参数即IPC_CREAT和IPC_EXCL来完成创建消息队列和得到消息队列的操作,这里加上static只是想让这个函数在本文件中有效
{
	key_t key = ftok(".",0);   //第一个参数是路径名，第二个参数是随意给的一个,这里我如果使用的是"./queue"就错误
	//这里一开始写0x6666，然后错了
	if(key < 0)
	{
		printf("ftok error\n");
		return -1;
	}
	int queue = msgget(key,flag);   //key是生成的一个端口，后面的两个宏时固有的参数，0666是访问 的权限
	if(queue == -1)
	{
		printf("msgget error\n");
		return -2;
	}
	return queue;   //这里我开始返回值是0，，这个是错误的，应该返回的是消息队列id，因为这个id以后会用到
}

int creatMsgQueue()
{
	int ret = commMsgQueue(IPC_CREAT|IPC_EXCL|0666);
	if(ret < 0)
	{
		printf("creatMsgQueue error\n");
		return -1;
	}
	return ret;
}

int getMsgQueue()
{
	int ret = commMsgQueue(IPC_CREAT);
	if(ret < 0)
	{
		printf("getMsgQueue error\n");
		return -1;
	}
	return ret;
}

int desMsgQueue(int id)
{
	int flag = msgctl(id,IPC_RMID,NULL);
	if(flag < 0)
	{
		printf("delete error\n");
		return -1;
	}
	return 0;
}



int rcvMsg(int id,int who,char* recbuf)  //我们需要得到某一个队列中的消息，所以这里传入队列号，另外我们还需要传入一个类型，这个类型是确认某一个类型呢的
{
	msginfo buf;	
	int ret = msgrcv(id,&buf,sizeof(buf.mtext),who,0);
	strcpy(recbuf,buf.mtext);
	if(ret < 0)
	{
		printf("rcvMsg error\n");
		return -1;
	}
	return 0;
}


int sndMsg(int id,int who,char* sndbuf)  //标记要往哪个队列里面发，标记发送或者是接受的数据类型,who就是相当于标记数据类型
{
	int  ret;
	msginfo buf;
	buf.mtype = who;  //who是用来标记数据类型 的
	strcpy(buf.mtext,sndbuf);  //这个时候我们应该是从外面传入一个数据
	//int msqid = getMsgQueue();//这里不能这样使用，因为我们要往一个消息队列中发送数据的时候，应该在外面把这个队列的id传进来
	//
	ret = msgsnd(id,(void*)&buf,sizeof(buf.mtext),0);  //虽然在这个函数中没有使用到who，但是who传进来是为了标记
	   //上面的那个buf.mtype的    发数据的时候只需要标记自己的类型就好了，所以这个函数中并没有who选项用来标记数据的
	   //类型，但是在上面的取数据的时候却需要who这个选项 用来识别我应该取什么样子的数据类型
	if(ret < 0)
	{
		printf("sndMsg error\n");
		return -1;
	}
	return 0;
}





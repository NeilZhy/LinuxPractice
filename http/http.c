#include"http.h"


int startup(char *ip,int port)   //创建一个监听套接字
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		printf("socket error\n");
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port); 
	local.sin_addr.s_addr = inet_addr(ip); 
	int on=1;  //允许重新绑定
	if((setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
	{  
		perror("setsockopt failed");  
		exit(EXIT_FAILURE);  
    } 

	int fbind = bind(sock,(struct sockaddr*)&local,sizeof(local));	
	if(fbind<0)
	{
		printf("bind error\n");
	}
	int listensock = listen(sock,10);
	if(listensock<0)
	{
		printf("listen error\n");
	}
	//return listensock; 一开始我这里写错了，不应该是listensock，listensock只是给标识，就是看我是否监听成功了，所以这是命名的不规范
	return sock;

}

void request_404(int sock)
{
	char *path = "wwwroot/404.html";   //把路径该成了404路径
	struct stat ispath;   //struct stat是一个结构体，这个结构体是为了配合着下面的stat这个函数使用的
	if(stat(path,&ispath)<0)	//这个函数的作用就是就是判断path这个路径是否正确
	{
		return;
	}

	int fd = open(path,O_RDONLY);
	if(fd<0)
	{
		printf("404打开文件失败\n");
		return;
	}
	char buf[SIZE];
	memset(buf,0,SIZE);
	sprintf(buf,"HTTP/1.0 404 not found \r\n\r\n");  //sprintf是将字符串打印到数组中
     if(send(sock,buf,strlne(buf),0)<0)   //使用send函数来向客户端发送反馈数据
	 {
		 printf("sned error\n");
		 return;
	 }  //其实这里的send和sendfile函数和readwrite函数等作用是差不多的，只不过这里使用他们更加的高效
	 if(sendfile(sock,fd,NULL,sizeof(buf))<0)  //这个函数我不知道是想干嘛的
	 {
		 printf("sendfile error\n");
		 return;
	 }
	 close(fd);


}


void echo_error(int sock,int err_code)
{
	switch(err_code)
	{
		case 401:
			break;
		case 404:
			request_404(sock);   //专门处理404的函数
			break;
		case 503:
			break;
		case 500:
			break;
		default:
			break;
	}
}

static int get_line(int sock,char *buf,int len)
	//参数解析：sock就是我们获取的那个网络中的文件描述符
	//buf是我们想要读取的文件的数据，就是那个HTTP的报头消息，想 拿着它去分析
	//len是缓冲区的大小
{
	//传递参数的时候一定 要想着合法性，这里使用assert
	assert(buf);
	if(len < 0)
	{
		return 5;
	}
	char ch = '\0';
	int i = 0;
	

	//while里面就是一个一个的读取数据，然后对数据进行分析
	while(i < len-1 && ch!= '\n' )  //当它读到一个换行符的时候，说明一行就结束了，
		//还应该思考一个问题，就是下面的我们的具体实现部分
	{
		//因为我们想要读取sock里面的数据，这个时候，我们需要每次读到一个字符
		//都分析一下这个数据是什么内容，因为我们的数据每一行的结尾是以/r/n结尾，这个时候如果我们读取了 一个数据
		//我们的整体的逻辑就是，每次读取一个数据，然后对这个数据进行分析，如果是/r，然后对下一个数据进行判断，如果是
		///n，就直接复制给ch，然后把数据放置在buf中，如果不是/n，这个时候就让他等于/n，然后放置在buf中，这个时候while就结束了
		if(recv(sock,&ch,1,0))
		{
			if(ch == '\r')
			{
				if(recv(sock,&ch,1,MSG_PEEK)>0 && ch == '\n')
				{
					recv(sock,&ch,1,0);
				}
				else
				{
					ch = '\n';
				}
			}
			buf[i++] = ch;
		}
		else
		{
			ch = '\n';
		}
	}
	buf[i] = '\0';
	return i;
}



static int clear_header(int sock)
{
	char buf[SIZE];
	int ret = -1;
	do{
		ret = get_line(sock,buf,sizeof(SIZE));
	}while((ret != 1)&&(strcmp(buf,"\n") != 0));
	return ret;
}

static void echo_www(int sock,char *path,int _s)
{
	int fd = open(path,O_RDONLY);
	if(fd < 0)
	{
		echo_error(sock,404);
		return;
	}
	char buf[SIZE];
	sprintf(buf,"HTTP/1.0 200 OK \r\n\r\n");
	if(send(sock,buf,strlen(buf),0)<0)
	{
		echo_error(sock,404);
		return;
	}
	if(sendfile(sock,fd,NULL,_s)<0)
	{
		echo_error(sock,404);
		return;
	}
	close(fd);
}








//第三步：这个是接着主函数中的那个的
//我想做的事情是获取sock传递的数据里面的内容 
//其实就是HTTP中的所有的报头的信息，然后对信息就行 解析

int handler_sock(int sock)
{
	char buf[SIZE];
	int ret;
	//1.0 调用get_line函数是为了获取HTTP报文中的数据，这个函数写在了下面
	if(get_line(sock,buf,sizeof(buf))<0)
	{
		printf("get_line erroro\n");
		ret = 6;
	}
	//2.0此时已经收获了第一行的信息了
	printf("buf %s\n",buf);
	char method[METHOD_SIZE];   //为了把方法给提取出来进行 分析
	char url[URL_SIZE];   //将url提取出来进行分析
	memset(method,0,sizeof(method));
	memset(url,0,sizeof(url));
	int i = 0,j = 0;
	while(i<sizeof(buf)-1&&j<sizeof(method)-1&&!isspace(buf[i]))   //这一句是想提取第一行中的方法信息，应为第一个字段方法和第二个 字段url之间是有空格的，所以这里就是加上一个空格的判端
		//还有中间的判断是放置读取的数据超过了buf的长度
		//防止读取的数据超过了method的长度
	{
		method[j] = buf[i];
		i++;
		j++;   //没必要用两个变量控制
	}

	if(strcasecmp(method,"GET")&&strcasecmp(method,"POST"))   //函数的结果是，相同返回0，所以上面的判断是如果不其中一个不相等就判断出错，这个地方需要考虑的是GET方法和POST方法
	{
		echo_error(sock,404);   //报错函数处理404错误
		goto end;
	}
	if(isspace(buf[i])&&i<sizeof(buf)-1)
	{
		i++;
	}//更新i的值，是为了指向url的位置，因为前面呢已经把method方法给取过了
	j = 0;
	//将url的数据放置在字符串url中去
	while(j < sizeof(url)-1,&&i<sizeof(buf)-1,&&!isspace(buf[i]))
	{
		url[j] = buf[i];
		j++;
		i++;
	}
	printf("method :%s\n",method);
	printnf("url :%s \n",url);

	//上面已经获取了方法了，这个时候，我们需要获取url的一些参数，可能是客户端传递的一些数据
	char *query_string = NULL;
	int cgi = 0;
	if(strcasecmp(method,"POST") == 0)
	{
		//说明是POST方法，这个时候一定用到了cgi这程序了
		cgi = 1;
	}
	if(strcasecmp(method,"GET")==0)
	{
		query_string = url;
		while(*query_string != '?'&&*query_string != '\0')
		{
			query_string++;  //目的是为了让这个指针指向了？
		}
		if(*query_string == '?')
		{
			*query_string == '\0';  //这个时候我们已经把url中的第一个？后面的内容给标记出来了
			query_string++;
			cgi = 1;  //虽然是GET方法，但是也是有cgi模式的
			//只不过GET方法的传递参数是在url中的
		}
	}
	printf("query_string :%s\n".query_string);
	char path[SIZE];
	memset(path,0,SIZE);
	sprintf(path,"wwwroot%s",url);  //这个时候，url是已经被截断的，因为上面的query_string已经把一个部分给置'\0'
	if(path[strlen(path)-1]=='/')
	{
		strcat(path,"index.html");
	}
	struct stat ispath;
	printf("path :%s\n",path);
	if(stat(path,&ispath)<0)
	{
		printf("stat\n");
		printf("stat file error");
		echo_error(sock,404);
		goto end;
	}
	else //除去不是/的情况
	{
		if(ispath.st_mode & S_IFDIR) //判断是不是目录
		{
			strcat(path,"/index.html");
		}
		else if((ispath.st_mode & S_IXUSR) ||\
			(ispath.st_mode & S_IXGRP) ||\
				(ispath.st_mode & S_IXOTH))
		{
			cgi = 1;
		}
	}
	if(cgi)
	{
		//对于cgi模式，我们需要对参数进行处理，method记录的是请求方法，path记录的是请求资源，query_string记录的是参数
		ret = excu_cgi(sock,method,path,query_string);
	}
	else
	{
		//非cgi模式，需要把HTTP报头进行全部的访问完毕，放置后续出现以后的报文粘包的问题
		clear_header(sock);
		//接下来是非cgi版本的反馈直接把资源发送过去
		echo_www(sock,path,ispath.st_size);
	}
end:
	close(sock);
	return ret;

}




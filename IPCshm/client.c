#include"comm.h"


int main()
{
	int shmid = getShm(0);
	int i = 0;
	char* arr = (char*)shmat(shmid,NULL,0);
	while(1)
	{
		sleep(1);
		printf("%s\n",arr);
		i++;
		if(i==25)
			break;
	}
	int dt =  shmdt(arr);
	if(dt < 0)
	{
		perror("shmdt\n");
		return -1;
	}
	return 0;
}

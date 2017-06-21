#include "comm.h"

int main()
{
    int sem = creatSem(1);
	initSem(sem,0,1);
    if(fork()==0)
	
	{
	    int csem = getSem(0);	
		while(1){
			P(csem,0);
		printf("A");
		fflush(stdout);
		usleep(234234);
		printf("A ");
		fflush(stdout);
		usleep(792830);
		V(csem,0);
		}
	}
	else
	{
		while(1){
        P(sem,0);
		printf("B");
		fflush(stdout);
		usleep(349580);
		printf("B ");
		fflush(stdout);
		usleep(794024);
		V(sem,0);
		}
	}
	desSem(sem);
	return 0;
}

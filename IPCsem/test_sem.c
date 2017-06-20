#include "comm.h"

int main()
{
	int id = creatSem(2);
	sleep(4);
	desSem(id);
	return 0;
}

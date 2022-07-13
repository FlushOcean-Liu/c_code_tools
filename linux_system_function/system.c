#include <stdio.h>
#include <stdlib.h>

#define CMD_SIZE     1024

int main()
{
	int ret=0;
	char cmd[CMD_SIZE]={0};

	snprintf(cmd, CMD_SIZE, "ls -al");

	ret=system(cmd);
	if(-1!=ret && 1==WIFEXITED(ret) && 0==WEXITSTATUS(ret)){
		printf("[%s] execute success! ret:%d, WIFEXITED:%d,WEXITSTATUS:%d\n",
						cmd, ret, WIFEXITED(ret), WEXITSTATUS(ret));
	}else{
		printf("[%s] execute failed! ret:%d, WIFEXITED:%d,WEXITSTATUS:%d\n",
						cmd, ret, WIFEXITED(ret), WEXITSTATUS(ret));
    }


	return 0;
}

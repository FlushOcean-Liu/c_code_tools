#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

using namespace std;

#define MYFIFO  "/tmp/my_fifo"   

struct new_test{
	char name[20];
	int age;
	int high;
	int weight;
};

int main()
{
	char buff[1024];
	int rdfd;
	int ret=0;
	struct new_test* info=NULL;

	memset(buff, 0, 1024);

	rdfd = open(MYFIFO, O_RDONLY);
	if(rdfd < 0){
		std::cout << "open error ..." << std::endl;
		exit(-1);
	}	

	std::cout << "waiting for reading ..." <<std::endl;
	while(1){
		ret = read(rdfd, buff, 1024);
		if(ret == 0){
		  std::cout << "end of read ... "<<std::endl;
		  break;
		}

		std::cout << "process "<< getpid() <<" read :"<<buff<< std::endl;
		info=(struct new_test*)buff;

		std::cout <<"get person info:" <<std::endl;
		std::cout <<"get Name:"<<info->name<<std::endl;
		std::cout <<"get Age:"<<info->age<<std::endl;
		std::cout <<"get high:"<<info->high<<std::endl;
		std::cout <<"get Weight:"<<info->weight<<std::endl;
			
		memset(buff, 0 ,1024);
	}
	close(rdfd);
	exit(0);
}

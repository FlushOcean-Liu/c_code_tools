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
	if(access(MYFIFO, F_OK) == -1){
		int ret =  mkfifo(MYFIFO,0666);
		if( ret < 0 ){
			std::cout<<"mkfifo error ..." << std::endl;
			exit(-1);
		}
	}

	char buff[1024];
	memset(buff, 0, 1024);

	struct new_test *info=(struct new_test *)buff;

	int wrfd;
	std::cout << "wating for another process open the myfifo to reading..." << std::endl;

	wrfd = open(MYFIFO, O_WRONLY);
	if( wrfd == -1){
		std::cout << "open error ..." <<std::endl;
		exit(-1);
	}

	pid_t pid = getpid();
	std::cout << "process" << pid << "write:";


	while(1){
		std::cout << "\nPlease input person info:" <<std::endl;
		std::cout<<"Input Name:";
		std::cin>>info->name;
		std::cout<<"Input Age:";
		std::cin>>info->age;
		std::cout<<"Input High:";
		std::cin>>info->high;
		std::cout<<"Input Weight:";
		std::cin>>info->weight;

		write(wrfd,buff, 1024);
		memset(buff, 0, 1024);
		std::cout << "process" << pid << " write:"<<std::endl;
		std::cout << "\nSTART TO INPUT NEXT INFO "<<std::endl
		sleep(1);
	}

	close(wrfd);
	exit(0);
}

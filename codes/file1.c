#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
#include<sys/wait.h>

int main(int argc,char *argv[]){
	int fd;
	pid_t pid;
	char buf[5];
	struct stat statbuf;

	fd=open("foo.txt",O_WRONLY);
	if(fd<0){
		printf("error");
	}
	lseek(fd,0,SEEK_END);
	write(fd,&buf,1);
	close(fd);
	fd=open("foo.txt",O_RDONLY);
	lseek(fd,0,SEEK_END);
	read(fd,&buf,1);
	printf("%c\n",buf);
}

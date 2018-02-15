#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

#define DEF_PAGER "/bin/more"
#define MAXLINE 1024

int main(int argc,char*argv[])
{
	int n;
	int fd[2];
	pid_t pid;
	char *pager,*argv0;
	char line[MAXLINE];
	FILE *fp;

	if(argc!=2){
		fprintf(stderr,"usage:a.out <pathname>");
		exit(0);
	}
	if((fp=fopen(argv[1],"r"))==NULL){
		fprintf(stderr,"can't open %s",argv[1]);
		exit(0);
	}
	if(pipe(fd)<0){
		fprintf(stderr,"pipe error");
		exit(0);
	}
	if((pid=fork())<0){
		fprintf(stderr,"fork error");
		exit(0);
	}
	else if(pid>0){//parent
		close(fd[0]);//close read side
		while(fgets(line,MAXLINE,fp)!=NULL){
			n=strlen(line);
			if(write(fd[1],line,n)!=n){
				fprintf(stderr,"write error to pipe");
			}
		}
		if(ferror(fp)){
			fprintf(stderr,"fgets error");
			exit(0);
		}
		close(fd[1]);
		if(waitpid(pid,NULL,0)<0){
			fprintf(stderr,"wait pid error");
			exit(0);
		}
		exit(0);
	}
	else{//child
		close(fd[1]);//close write
		if(fd[0]!=STDIN_FILENO){//change standard input to pipe
			if(dup2(fd[0],STDIN_FILENO)!=STDIN_FILENO){
				fprintf(stderr,"dup2 error to stdin");
			}
			close(fd[0]);
		}
		if((pager=getenv("PAGER"))==NULL){
			pager=DEF_PAGER;
		}
		if((argv0=strrchr(pager,'/'))!=NULL){
			argv0++;
		}
		else{
			argv0=pager;
		}
		if(execl(pager,argv0,(char*)0)<0){
			fprintf(stderr,"excl error for pager");
		}
		exit(0);
	}
}

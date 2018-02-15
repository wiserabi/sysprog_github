#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

typedef struct{
	long mtype;
	char mtext[20];
}MsgType;

int main(void)
{
	key_t key=4071;
	int que_id=msgget(key,IPC_CREAT|0666);
	int get_type;
	MsgType msg;
	int msg_size=sizeof(msg)-sizeof(msg.mtype);
	ssize_t nbytes=0;
	
	printf("please insert wanted MSG type(1 or 2)\n");
	scanf("%d",&get_type);

	nbytes=msgrcv(que_id,&msg,msg_size,get_type,IPC_NOWAIT);
	if(nbytes>0){
		printf("%s\n",msg.mtext);
	}
	else if(errno==ENOMSG){
		printf("empty queue\n");
	}
	return 0;
}

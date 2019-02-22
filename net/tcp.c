#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "mid_report.h"
#include <pthread.h>


//#define SERVER_IP	"10.206.24.170"
//#define SERVER_IP	"10.206.24.127"
#define SERVER_IP  	"10.206.25.218"
#define SERVER_PORT 8888
extern env_t env;



void report(void)
{
	if(env.flag.net_need_send == 1){

	}else{
		
	}

}


void* net_work(void * arg)
{
	int ret; 
	char buf[3];
	msg_t msg;	
	int len,size;
	int sockfd;
	struct sockaddr_in servaddr;


	printf("net init!\n");

	/*create the socket*/
	if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
	{
		perror("socket error");
		exit(1);
	}

	/*the addr and type*/
	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);
	servaddr.sin_addr.s_addr=inet_addr(SERVER_IP);
	

	/*connect*/
	if( connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
	{
		perror("connect error");
		exit(1);
	}//if
	

	while(1)	
	{
		pthread_mutex_lock(&env.mutex);
		pthread_cond_wait(&env.cond, &env.mutex);
		size = sizeof(env.serial_task.buf);
		len = strlen( env.serial_task.buf); 
		env.serial_task.buf[len] = '\0';
		printf("net send:%s size=%d len=%d\n",env.serial_task.buf,size,len);
		ret = write(sockfd ,env.serial_task.buf,len) ;
		pthread_mutex_unlock(&env.mutex);
		
		

	}//while
	

	/*close*/
	close(sockfd);

}

/*
	This file is the main file of this project.
	
												haobo@2018年11月15日
*/
#include "stdio.h"
#include "tcp.h"
#include "logic.h"
#include "serial.h"
#include "mid_report.h"
#include "string.h"
#include "stdlib.h"
#include <pthread.h>

/* global var */
env_t env;

void print_info(void)
{
	printf("\n\n\n\n********* This is a applicition for UWB date reporter**********\n");
	printf("* Code was writen by haobo ,\t*\n*any problem please contact:\t *\n* \t\thao-bo.gao@foxconn.com*\n\n");
	
}

int process_par(int argc,char* argv[]) 
{
	int i;
	if(argc <= 1){
		printf("No argument detected,please input a tty device at least\n");
		printf("fomat like: /dev/tty*** \n");
		return -1;
	}else{
		printf("detected user input:\n");
		for(i = 0;i<argc;i++){
			printf("arg %d: %s\n",i,argv[i]);
			if(strncmp("/dev/tty",argv[i],8)==0){
				env.user_arg.ttydev_name = (char*) malloc(sizeof(argv[i]));
				strcpy(env.user_arg.ttydev_name,argv[i]);	
			}
		}
		
	  printf("\nuse %s as serial device \n",env.user_arg.ttydev_name);
	}	
}



void free_resouce(void)
{
	free(env.user_arg.ttydev_name);
}

int main(int argc,char* argv[])
{
	print_info();

	/* process the arg */
	if (process_par(argc,argv) == -1)
		return -1;	//handle  args the user send in

	pthread_t  tid_serial,tid_net;
	logic_init();
	
	pthread_create(&tid_serial, NULL, &serial_handle, NULL);
	pthread_create(&tid_net, NULL, &net_work, NULL);

	pthread_join(tid_serial, NULL);
	pthread_join(tid_net, NULL);
	free_resouce();
}

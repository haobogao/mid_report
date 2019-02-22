/*
 * Create by haobo for serial data support
 *
 * 2018 11.15
 *
 */
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "serial.h"
#include "mid_report.h"
#include <pthread.h>


extern env_t env;
struct serial_device serial_dev;



void tick_net(struct serial_data_struct * sdata)
{
	pthread_mutex_lock(&env.mutex);
    sprintf(env.serial_task.buf,"%s,%d;0,%d,1,%d,2,%d,3,%d",sdata->time_str,sdata->tag_id,sdata->dst_to_A0,sdata->dst_to_A1 ,sdata->dst_to_A2,sdata->dst_to_A3);
	pthread_mutex_unlock(&env.mutex);
	pthread_cond_signal(&env.cond);
}


/* 
	initalize the serial_data_struct ,give it the time val,and 
	the initial value
*/
void initalize_serial_data(struct serial_data_struct ** sdata)
{
	struct timeval  tv;
	struct timezone tz;
	struct tm * tp;
	time_t seconds;
	// -1 tell us this value is not effective
	(*sdata)->dst_to_A0 = 0;
	(*sdata)->dst_to_A1 = 0;
	(*sdata)->dst_to_A2 = 0;
	(*sdata)->dst_to_A3 = 0;
	seconds = time((time_t *)NULL);
	tp = localtime(&tv.tv_sec);
	sprintf((*sdata)->time_str,"%d",seconds);		//tv.tv_usec/1000,tv.tv_usec%1000
	(*sdata)->time_strlen =  strlen((*sdata)->time_str);
}


/* 	
	divied the serial data by a single space.
	after this step the data should be saved in
	array.
*/
struct serial_data_struct *  cut_by_space(char * str,int len)

{
	int i,num,j;
	char *p,*temp;
	struct serial_meta_data  mdata[50] = {0};
	struct serial_data_struct * sdata = NULL;
	i = 0;
	//cut and load
	p = strtok(str," ");
	while(p!=NULL){

		strcpy(mdata[i].data ,p);
		i++;
		p = strtok(NULL," ");

	}
	num = i;

	for(i=0;i<num;i++){
		// base station to tag
		if( strcmp(mdata[i].data,"mc")== 0 ){ //|| 
			sdata = (struct serial_data_struct *)malloc(sizeof(struct serial_data_struct));
			printf("%s\n",mdata[i].data);
			sdata->mask = strtol(mdata[i+1].data,NULL,16);
			initalize_serial_data(&sdata);
			//range 1 effective
			if(sdata->mask & 1){
				sdata->dst_to_A0 = strtol(mdata[i+2].data,NULL,16);
			}
			//range 2 effective
			if(sdata->mask & 2){
				sdata->dst_to_A1 = strtol(mdata[i+3].data,NULL,16);
			}
			//range 3 effective
			if(sdata->mask & 4){
				sdata->dst_to_A2 = strtol(mdata[i+4].data,NULL,16);
			}

			//range 4 effective
			if(sdata->mask & 8){
				sdata->dst_to_A3 = strtol(mdata[i+5].data,NULL,16);
			}
			
			sdata->tag_id = strtol(mdata[9].data+1,&p,16);		//tag id
			p = p+1;
			sdata->bs_id =  strtol(p,NULL,16);
			//printf("after convert:\t time: %s,tag id:%d,A0,%d,A1,%d,A2,%d\n",sdata->time_str,sdata->tag_id,sdata->dst_to_A0,sdata->dst_to_A1 ,sdata->dst_to_A2);
			return &(*sdata);
		}
		
	}

	

	return NULL;
}



//init the serial port
void* serial_handle(void * arg)
{
	int res,offset,len,loop;
	static unsigned long countp=0,counts=0;
	char * p;
	char buf[500];
	char** array;
	struct serial_data_struct * sdata;
	printf("serial init started!\n");
	serial_dev.fd = open(env.user_arg.ttydev_name,O_RDWR|O_NOCTTY);
	if(serial_dev.fd < 0){
		perror(CONSOLE_TTY);
		exit(-1);
	}
	serial_dev.tio.c_iflag = IGNPAR | ICRNL;
	serial_dev.tio.c_cflag = BAUDRATE|CRTSCTS|CS8|CLOCAL|CREAD;
	serial_dev.tio.c_oflag = 0;
	serial_dev.tio.c_lflag = ICANON;

	/*
		now clean the modem line and activate the settings for the port
	*/
	tcflush(serial_dev.fd , TCIFLUSH);
	tcsetattr(serial_dev.fd,TCSANOW,&serial_dev.tio);

	/*
		terminal settings done, now handle input
		In this example, inputting a 'z' at the beginning of a line will
		exit the program.
	*/
	while (1) {
		bzero(buf,sizeof(buf));
		res = read(serial_dev.fd,buf,500);
		buf[res]=0; /* set end of string, so we can printf */
		env.flag.recvd_not_handle =1;
		env.flag.net_need_send = 0;
		if(res < 37){
			printf("noise\n");
			continue;
		}
			
		if(res>=37)
			printf("\n\nrecevice: %s \nlen:%d\n",buf,res);
		/* data pretreatment */		
		sdata = cut_by_space(buf,res);
		countp++;
		if(sdata == NULL){
			free(sdata);
			continue;
		}
		if(sdata->dst_to_A0 != 0 && sdata->dst_to_A1 != 0 && sdata-> dst_to_A2 != 0){
			tick_net(sdata);
			counts++;
		}
		free(sdata);
		printf("process:%d,send:%d\n",countp,counts);
	}
}






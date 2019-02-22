#ifndef __MID_REPORT_H__
#define __MID_REPORT_H__
#include "serial.h"
#include "logic.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/shm.h>





#define SHM_ID 1234
#define DATA_TYPE_SRC_TAG_BS 1
#define DATA_TYPE_OP_TAG_BS  2


#define  CONFIG_MAX_TAG_ID 8

typedef  struct msg_struct{
	unsigned char type;
	char buf[100];
} msg_t;



/* discripe the user arg */
struct user_arg_struct {
	unsigned char * ttydev_name;
};

struct task_serial{
	pid_t pid;
	char buf[100];
};


/*  
	Here define a status machine.
	when there's new data comming:

	recvd_not_handle=1  net_need_send=0
		     |
		    \|/
	 serial_handle
	    	|
	       \|/
	net_need_send=1    recvd_not_handle=0

		 |
		\|/
	 net write
	 	|
	   \|/
	net_need_send=0   recvd_not_handle=0

*/
typedef struct flag_struct{
	unsigned char net_need_send:1;		//there is a data need transfer to server
	unsigned char recvd_not_handle:1;	
	
} flag_t;

struct task_net{
	pid_t pid;
};


/*  the whole program global data type */
typedef struct env_struct {
	flag_t flag;
	struct user_arg_struct user_arg;
	struct task_net		net_task;
	struct task_serial	serial_task;
	pthread_mutex_t  mutex;
	pthread_cond_t cond;
} env_t;

#endif



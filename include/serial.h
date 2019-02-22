#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <termios.h>
/* lock to the correct device during develop  */
#define CONSOLE_TTY		"/dev/ttyACM7"
#define BAUDRATE 		B115200



#define FALSE 0
#define TRUE 1



struct serial_device{
	int fd;
	struct termios tio;
	char * buffer;
};


struct serial_data_struct {
	int dst_to_A0;
	int	dst_to_A1;
	int	dst_to_A2;
	int	dst_to_A3;
	char  time_str[50];
	int time_strlen;
	unsigned short mask;
	int tag_id;
	int bs_id;
};


struct serial_meta_data{
	char data[100];
	int len;
};




void* serial_handle(void * arg);


#endif

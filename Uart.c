#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include "Uart.h"

int uart0_filestream;

void uartSendMessage(char * tx_buffer, char*append , int uart0_filestream){
	char result[200];
	snprintf(result, sizeof(result), "%s %s", tx_buffer, append);
	if(uart0_filestream != -1){
                int count = write(uart0_filestream, result, 255);
                if(count < 0)
                {
                        printf("UART TX error\n");
                }
        }
}


int uartInit(){
        int uart_filestream = -1;

        uart_filestream = open("/dev/serial0",O_RDWR | O_NOCTTY | O_NDELAY);

        if(uart_filestream == -1){
                printf("Error - Unable to open UART. Ensure it is not use by another applicaion\n");
        }

        struct termios options;
        tcgetattr(uart_filestream,&options);
        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(uart_filestream, TCIFLUSH);
        tcsetattr(uart_filestream, TCSANOW, &options);

	return uart_filestream;
}

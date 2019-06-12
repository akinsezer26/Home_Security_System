#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h> /*THREAD*/
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

typedef int bool;

#define TRUE 1
#define FALSE 0
#define true 1
#define false 0

int client;

int count=100;
int rCount=0;

int port;
char ip[16];
bool bye=false;

int sensorDurum=0;
int surucuDurum=0;
char surucuAcKapa;
int Uart=false;

int uart0_filestream;

void signal_handler(int no){
	char str[100];
	sprintf(str,"bye");

	if(write(client,str,strlen(str))!=strlen(str)){
		printf("Error writing data [%s]\n",strerror(errno));
	}
	close(client);
	exit(0);
}

void *Thread1_ReceiveMessage(void *parameter)
{
	char str[100];
	while(!bye && !Uart){
		memset(str,'\0',100);
		if((rCount=read(client, str, count))==0){
			printf("Error reading data [%s]\n",strerror(errno));
			return;
		}
		printf("\nGelen Yanit	  : %s\n",str);

		if(!strncmp(str,"bye",3))
			bye=true;

		else if(!strncmp(str,"SensorDurum :  ",15)){
			sensorDurum=str[15]-48;
		}

		else if(!strncmp(str,"SurucuDurum :  ",15)){
                        surucuDurum=str[15]-48;
                }

	}
}

int uartInit(){
        int uart_filestream = -1;

        uart_filestream = open("/dev/serial0",O_RDWR | O_NOCTTY | O_NDELAY);

        if(uart_filestream == -1){
                printf("Uart Bagli Degil!! \n");
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


void uartSendMessage(char * tx_buffer){
        if(uart0_filestream != -1){
                int count = write(uart0_filestream, tx_buffer , 255);
                if(count < 0)
                {
                        printf("UART TX error\n");
                }
        }
}

void sendMessage(char*message){

	if(write(client,message,strlen(message))!=strlen(message) && !Uart){
        	printf("Error writing data [%s]\n",strerror(errno));
	}

	if(uart0_filestream!=-1){
		uartSendMessage(message);
	}
}

void  menu(){
	char str[100];
	char secim1;
	char secim2;
	char sensorKontrol='n';
	char denetim='n';
	printf("\n+==========================================+\n");
	printf("|                 ANA MENU		   |\n");
	printf("| 1) Sensor				   |\n");
	printf("| 2) Surucu	                           |\n");
	printf("| 3) Denetim Modu                          |\n");
	printf("| 4) Mesaj Gir		                   |\n");
	printf("| 5) Kapat                                 |\n");
	printf("+==========================================+\n\n");
	printf(" Seciminiz =========> ");

	scanf(" %c",&secim1);

	switch(secim1){
		case 49:
			printf("\n+==========================================+\n");
               		printf("|                  SENSOR                  |\n");
               		printf("| 1) Geri                                  |\n");
               		printf("| 2) Sensor Tipi                           |\n");
               		printf("| 3) Sensor Durumu                         |\n");
               		printf("+==========================================+\n\n");
               		printf(" Seciminiz =========> ");

			scanf(" %c",&secim2);

			switch(secim2){
				case 49:
					break;
				case 50:
					sendMessage("SensorTip");
					sleep(1);
                        		printf("\nDevam etmek için bir tusa basin...\n");getchar();getchar();
					break;
				case 51:
					sendMessage("SensorDurum");
					sleep(1);
                        		printf("\nDevam etmek için bir tusa basin...\n");getchar();getchar();
					break;
				default:
					printf("\nLutfen gecerli bir komut secin\n");
					sleep(1);
                        		printf("\nDevam etmek için bir tusa basin...\n");getchar();getchar();
					break;
			}
			break;

		case 50:
			printf("\n+==========================================+\n");
                        printf("|                  SURUCU                  |\n");
                        printf("| 1) Geri                                  |\n");
                        printf("| 2) Surucu Tipi                           |\n");
                        printf("| 3) Surucu Durumu                         |\n");
			printf("| 4) Surucu Durumunu Degistir              |\n");
                        printf("+==========================================+\n\n");
                        printf(" Seciminiz =========> ");

			scanf(" %c",&secim2);

			switch(secim2){
				case 49:
					break;
				case 50:
					sendMessage("SurucuTip");
					sleep(1);
                        		printf("\nDevam etmek için bir tusa basin...\n");getchar();getchar();
					break;
				case 51:
					sendMessage("SurucuDurum");
					sleep(1);
                        		printf("\nDevam etmek için bir tusa basin...\n");getchar();getchar();
					break;
				case 52:
					sendMessage("SurucuDurum");
					sleep(1);
					if(surucuDurum==1){
						printf("\nSurucu su anda acik kapatmak icin '0' tusuna basin\n");
						scanf(" %c",&surucuAcKapa);
						if(surucuAcKapa==48){
							printf("\nSurucu Kapatildi!\n");
							sendMessage("Surucu 0");
						}
						else
							printf("\nSurucu Durumu Degismedi\n");
						}
					else{
						printf("\nSurucu su anda kapali acmak icin '1' tusuna basin");
                                                scanf(" %c",&surucuAcKapa);
                                                if(surucuAcKapa==49){
                                                        printf("\nSurucu Acildi!\n");
							sendMessage("Surucu 1");
						}
                                                else
                                                        printf("\nSurucu Durumu Degismedi\n");
					}
                        		printf("\nDevam etmek için bir tusa basin...\n");getchar();getchar();
					break;
				default:
					printf("\nLutfen gecerli bir komut secin\n");
					sleep(1);
                        		printf("\nDevam etmek için bir tusa basin...\n");getchar();getchar();
					break;
			}
			break;

		case 51:
			printf("Sensor Listesi:\n");
			sendMessage("SensorTip");
			sleep(1);
			printf("Sensorun hangi durumunda surucu ON olsun?(1-ON/0-OFF)");
			scanf(" %c",&sensorKontrol);
			printf("\nSu anda denetim acik Sensor ON ise, surucu de ON olacaktir. Cikmak icin�'e' ye basiniz...");

			while(denetim!='e'){
				denetim=getchar();
				sendMessage("SensorDurum");
				if(sensorKontrol==48){
					if(sensorDurum==0)
						sendMessage("Surucu 1");
					else
						sendMessage("Surucu 0");
				}
				else if(sensorKontrol==49){
					if(sensorDurum==1)
                                                sendMessage("Surucu 1");
                                        else
                                                sendMessage("Surucu 0");
				}
				else{
					printf("\nLutfen gecerli bir komut secin\n");
				}
				sleep(1);
			}
                        printf("\nDevam etmek için bir tusa basin...\n");getchar();getchar();
			break;

		case 52:
			memset(str,'\0',100);
                        printf("Mesaj Yaz          : ");
                        scanf(" %[^\t\n]s",str);
                        sendMessage(str);
			break;

		case 53:
			sendMessage("Kapat");
			close(client);
			sleep(1);
                        printf("\nDevam etmek için bir tusa basin...\n");getchar();getchar();
			exit(0);
			break;

		default:
			printf("\nLutfen gecerli bir komut secin\n");
			printf("\nDevam etmek için bir tusa basin...\n");getchar;getchar();
			break;
	}
}

void * Thread2_menuGoster(void * parameter){
	while(!bye){
		menu();
		system("clear");
	}
}

int main(int argc, char **argv){
	/*sinyali aktif hale getir.*/
	struct sigaction newAction, oldAction;
	struct sockaddr_in caddr;
	newAction.sa_handler=signal_handler;
	newAction.sa_flags=SA_NODEFER |SA_ONSTACK;
	sigaction(SIGINT, &newAction, &oldAction);
	if(argc!=5){
		printf("False arguments...\n");
		Uart=true;
	}
	/*Argumanlara bak.*/
	int i;
	if(!Uart){
		for(i=1;i<4;i++){
			if(!strncmp(argv[i],"-port",5)){
				port=atoi(argv[i+1]);
			}

			if(!strncmp(argv[i],"-ip",3)){
				strcpy(ip,argv[i+1]);
			}
		}


		/*Adresi olusturalim.*/
		caddr.sin_family=AF_INET;
		caddr.sin_port=htons(port);
		if(inet_aton(ip,&caddr.sin_addr)==0){
		return (-1);
		}

		/*Sunucu olarak baglanti kuracak soketi yaratalim.*/
		client=socket(PF_INET, SOCK_STREAM, 0);
		if(client==-1){
			printf("Error on socket creation [%s]\n",strerror(errno));
			return (-1);
		}

		/*Suucuya baglanti istegi gonderip, baglantiyi kuralim.*/
		if(connect(client, (const struct sockaddr *)&caddr,sizeof(struct sockaddr))==-1){
			printf("Error on socket connect [%s]\n",strerror(errno));
			return (-1);
		}
	}

	uart0_filestream=uartInit();

	sleep(1);
	pthread_t threadId1, threadId2;
	pthread_create(&threadId1, NULL, Thread1_ReceiveMessage, NULL);
	pthread_create(&threadId2, NULL, Thread2_menuGoster, NULL);
	pthread_join(threadId1, NULL);
	pthread_join(threadId2, NULL);

	/*Soketleri kapatalim.*/
	close(client);
	return 0;
}

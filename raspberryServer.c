#include <pthread.h> /*THREAD*/
#include "Sensor.h"
#include "Sunucu.h"
#include "Surucu.h"
#include "Uart.h"

typedef int bool;

#define TRUE 1
#define FALSE 0
#define true 1
#define false 0
#define ON 1
#define OFF 0

int client, server;
int count=100;
int rCount=0;
int port;
bool bye=false;

bool Uart=false;

sensor*sensor1;
surucu*surucu1;

struct sockaddr_in saddr,caddr;

char sifre[4]={'0','0','0','0'};

void signal_handler(int no){
	char str[100];
	sprintf(str,"bye");

	if(write(client,str,strlen(str))!=strlen(str)){
		printf("Error writing data [%s]\n",strerror(errno));
	}
	//close(server);
	close(client);
	exit(0);
}

int uart0_filestream;

void *Thread1_mesajAl(void *parameter)
{
	char str[100];
	char tmp[25];

	while(!bye && ! Uart){
		memset(str,'\0',100);

		if((rCount=read(client, str, count))==0){
			printf("Error reading data [%s]\n",strerror(errno));
			close(client);
			client=clientBekle(server,client,caddr);
		}

		printf("Gelen Mesaj    : %s\n",str);

		if(!strncmp(str,"Kapat",5)){
                        bye=true;
			close(client);
                        exit(0);
                }

		else if(!strncmp(str,"Surucu 0",8)){
			surucuDurumunuDegistir(27,surucu1,OFF);
		}

		else if(!strncmp(str,"Surucu 1",8)){
			surucuDurumunuDegistir(27,surucu1,ON);
		}

		else if(!strncmp(str,"SensorTip",9)){
			mesajGonder(client,"SensorTip : ",sensorTipiniAl(sensor1));
		}

		else if(!strncmp(str,"SensorDurum",11)){
			snprintf(tmp,25,"%d",sensorDurumunuAl(sensor1));
			mesajGonder(client,"SensorDurum : ",tmp);
		}

		else if(!strncmp(str,"SurucuTip",9)){
			mesajGonder(client,"SurucuTip : ",surucuTipiniAl(surucu1));
		}

		else if(!strncmp(str,"SurucuDurum",11)){
                        snprintf(tmp,25,"%d",surucuDurumunuAl(surucu1,27));
                        mesajGonder(client,"SurucuDurum : ",tmp);
                }

	}
	return;
}

void *Thread2_sensorDegeriniOku(void *parameter)
{
	sensorDurumunuGuncelle(17, 1, sensor1);
}

void *Thread3_keypadDinle()
{
	char keypressed='n';
        char prevKeypressed=keypressed;
	char pressedKeys[10];
	int i=0;
	int flag;
        while(TRUE){
		flag=0;
                prevKeypressed=keypressed;
                keypressed=readKeypad();
                switch(keypressed){
                        case 'e':
                                break;
                        case 'n':
                                break;
                        default:
                                if(prevKeypressed=='n'){
                                        printf("Key:%c \n",keypressed);
					pressedKeys[i]=keypressed;
					if(i==10)
						i=0;
					if(i==6 && pressedKeys[0]==sifre[0] && pressedKeys[1]==sifre[1] && pressedKeys[2]==sifre[2] && pressedKeys[3]==sifre[3] && pressedKeys[4]=='*' && pressedKeys[5]=='0' && pressedKeys[6]=='#'){
                                        	surucuDurumunuDegistir(27,surucu1,OFF);
						printf("\nSurucu Kapatildi\n\n");
						flag=1;
						i=0;
						break;
	                                }
        	                        else if(i==6 && pressedKeys[0]==sifre[0] && pressedKeys[1]==sifre[1] && pressedKeys[2]==sifre[2] && pressedKeys[3]==sifre[3] && pressedKeys[4]=='*' && pressedKeys[5]=='1'&& pressedKeys[6]=='#'){
                	                        surucuDurumunuDegistir(27,surucu1,ON);
						printf("\nSurucu Acildi\n\n");
						flag=1;
                                                i=0;
						break;
                        	        }

                                	else if(i==9 && pressedKeys[0]==sifre[0] && pressedKeys[1]==sifre[1] && pressedKeys[2]==sifre[2] && pressedKeys[3]==sifre[3] && pressedKeys[4]=='*' && pressedKeys[9]=='#'){
                                        	sifre[0]=pressedKeys[5];
                                        	sifre[1]=pressedKeys[6];
                                        	sifre[2]=pressedKeys[7];
                                        	sifre[3]=pressedKeys[8];
                                        	printf("\nSifre Degistirildi !!!\n");
                                        	printf("\nYeni Sifre %c%c%c%c\n\n",sifre[0],sifre[1],sifre[2],sifre[3]);
						flag=1;
						i=0;
						break;
                            		}
					else if(i==9 && !flag){
                                                printf("\nGecersiz islem !! Lutfen gecerli bir komut giriniz \n\n");
                                                i=0;
						break;
                                        }
				i++;
				}
			break;
			}
        }
}


void *Thread4_uartMesajAl(void *parameter)
{
        char str[256];
        char tmp[25];

        while(!bye && uart0_filestream != -1){
          	int rx_length = read(uart0_filestream, (void*)str,255);
		sleep(1);
		if(rx_length>0){
                        str[rx_length] = '\0';
			printf("Gelen Mesaj    : %s\n",str);
		}

                if(!strncmp(str,"Kapat",5)){
                        bye=true;
                        exit(0);
                }

                else if(!strncmp(str,"Surucu 0",8)){
                        surucuDurumunuDegistir(27,surucu1,OFF);
                }

                else if(!strncmp(str,"Surucu 1",8)){
                        surucuDurumunuDegistir(27,surucu1,ON);
                }

                else if(!strncmp(str,"SensorTip",9)){
                        uartSendMessage("SensorTip : ", sensorTipiniAl(sensor1) , uart0_filestream);
			sleep(1);
                }

                else if(!strncmp(str,"SensorDurum",11)){
                        snprintf(tmp,25,"%d",sensorDurumunuAl(sensor1));
                        uartSendMessage("SensorDurum : ", tmp , uart0_filestream);
			sleep(1);
                }

	        else if(!strncmp(str,"SurucuTip",9)){
                        uartSendMessage("SurucuTip : ", surucuTipiniAl(surucu1) , uart0_filestream);
			sleep(1);
                }

                else if(!strncmp(str,"SurucuDurum",11)){
                        snprintf(tmp,25,"%d",surucuDurumunuAl(surucu1,27));
                        uartSendMessage("SurucuDurum : ", tmp , uart0_filestream);
			sleep(1);
                	}

        }
        return;
}



int main(int argc, char **argv){
	/*sinyali aktif hale getir.*/

	sensor1=sensorDegerAta("Hareket");
	surucu1=surucuDegerAta("Role");	

	struct sigaction newAction, oldAction;
        newAction.sa_handler=signal_handler;
        newAction.sa_flags=SA_NODEFER |SA_ONSTACK;
        sigaction(SIGINT, &newAction, &oldAction);

	uart0_filestream=uartInit();

	if(argc!=5){
		printf("False arguments...\n");
		Uart=true;
	}

	if(!Uart){
	/*Argumanlara bak.*/
		int i;
		for(i=1;i<2;i++){
			if(!strncmp(argv[i],"-port",5)){
				port=atoi(argv[i+1]);
			}
		}

		/*Baglanti olustur.*/
		/*Adresi olusturalim.*/
        	saddr.sin_family=AF_INET;
        	saddr.sin_port=htons(port);
        	saddr.sin_addr.s_addr=htonl(INADDR_ANY);

		/*Sunucu olarak baglanti isteklerini dinleyecek soketi yaratalim.*/
        	server=socket(PF_INET, SOCK_STREAM, 0);

		if(server==-1){
			printf("Error on socket creation [%s]\n",strerror(errno));
			return false;
		}

		/*Sokete olusturdugumuz adresi ve ayarlari baglayalim.*/
		if(bind(server,(struct sockaddr *)&saddr,sizeof(struct sockaddr))==-1){
			printf("Error on socket bind [%s]\n",strerror(errno));
			return false;
		}
	}

	printf("\n+==========================================+\n");
        printf("|                 SUNUCU                   |\n");
        printf("+==========================================+\n\n");

	if(!Uart)
		client=clientBekle(server,client,caddr);

	//close(server);
	sleep(1);

	pthread_t threadId1, threadId2 ,threadId3 ,threadId4;
        pthread_create(&threadId1, NULL, Thread1_mesajAl, NULL);
        pthread_create(&threadId2, NULL, Thread2_sensorDegeriniOku, NULL);
        pthread_create(&threadId3, NULL, Thread3_keypadDinle, NULL);
        pthread_create(&threadId4, NULL, Thread4_uartMesajAl, NULL);
	pthread_join(threadId1, NULL);
	pthread_join(threadId2, NULL);
	pthread_join(threadId3, NULL);
	pthread_join(threadId4, NULL);

	/*Soketleri kapatalim.*/
	//close(client);
	return 0;
}


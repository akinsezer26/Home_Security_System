#include "Sunucu.h"

int clientBekle(int server,int client,struct sockaddr caddr)
{
        int error = 0;
        socklen_t len = sizeof (error);

        /*Baglanti istekleri icin dinlemeye baslayalim.*/
        if(listen(server, 5)==-1){
                printf("Error on socket listen [%s]\n",strerror(errno));
                return (-1);
        }

        /*Baglanti talebi geldi, baglantinin idamesini yeni bir sokete tahsisedelim.*/
        uint32_t inet_len=sizeof(caddr);
        if((client = accept(server,(struct sockaddr *)&caddr, &inet_len))==-1){
                printf("Error on socket accept [%s]\n",strerror(errno));
                return 0;
        }

	return client;
}

int mesajGonder(int client,char*string1,char*string2){
	char result[200];
	snprintf(result, sizeof(result), "%s %s", string1, string2);
        if(write(client,result, strlen(result))!=strlen(result)){
		printf("Error writing data [%s]\n",strerror(errno));
		return (-1);
                }
	return 0;
}

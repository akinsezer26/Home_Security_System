#include "Surucu.h"
#include <stdlib.h>

surucu * surucuDegerAta(char * surucuTipi){
	surucu*p=malloc(sizeof(surucu));
        p->surucuTipi=surucuTipi;
        return p;
}

void surucuDurumunuDegistir(int pin, surucu*s,int acKapa){
	if(acKapa==1){
		GPIOWrite(pin,HIGH);
	}

	else{
		GPIOWrite(pin,LOW);
	}
}

unsigned int surucuDurumunuAl(surucu*s,int pin){
	s->surucuDurum = GPIORead(pin);
	return (s->surucuDurum);
}

char * surucuTipiniAl(surucu*s){
	return (s->surucuTipi);
}


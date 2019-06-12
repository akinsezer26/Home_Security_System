#include "Sensor.h"

sensor * sensorDegerAta(char * sensorTipi){
	sensor*p=malloc(sizeof(sensor));
	p->sensorTipi=sensorTipi;
	return p;
}

void sensorDurumunuGuncelle(int pin, int delay, sensor*s){ 
	while(1){
                s->sensorDurum = GPIORead(pin);
                sleep(delay);
        }
}

unsigned int sensorDurumunuAl(sensor*s){
	return (s->sensorDurum);
}

char * sensorTipiniAl(sensor*s){
	return (s->sensorTipi);
}


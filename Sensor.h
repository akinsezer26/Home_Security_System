#ifndef SENSOR_H
#define SENSOR_H
#include "gpioDIO.h"

typedef struct
{
	unsigned int sensorDurum;
	char * sensorTipi;
}sensor;

sensor * sensorDegerAta(char * sensorTipi);

void sensorDurumunuGuncelle(int pin, int delay, sensor*s);

unsigned int sensorDurumunuAl(sensor*s);

char * sensorTipiniAl(sensor*s);

#endif

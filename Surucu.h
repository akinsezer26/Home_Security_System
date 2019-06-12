
#ifndef SURUCU_H
#define SURUCU_H
#include "gpioDIO.h"

typedef struct
{
        unsigned int surucuDurum;
        char * surucuTipi;
}surucu;

surucu * surucuDegerAta(char * surucuTipi);

void surucuDurumunuDegistir(int pin, surucu*s,int acKapa);

unsigned int surucuDurumunuAl(surucu*s,int pin);

char * surucuTipiniAl(surucu*s);

#endif

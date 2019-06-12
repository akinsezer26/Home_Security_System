#include "gpioDIO.h"

char key[4][4]={{'1','2','3','A'},
                {'4','5','6','B'},
                {'7','8','9','C'},
                {'*','0','#','D'}};

int keyGPIOCol[4]={6,13,19,26};
int keyGPIORow[4]={16,20,21,12};

char readKeypad(){
        int k,i,j;
        for(i=0;i<4;i++){
                //Set high first column
                if(GPIOWrite(keyGPIOCol[i],1)==-1)
                        return 'e';

                for(j=0;j<4;j++){
                        if((k=GPIORead(keyGPIORow[j]))==-1)
                                return 'e';
                        if(k==1)
                                return key[j][i];
                }

                if(GPIOWrite(keyGPIOCol[i],0)==-1)
                        return 'e';
        }
        return 'n';
}

#include<xc.h>
#define _XTAL_FREQ 1000000
#include "LibLCDXC8.h"
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF

void main(void){
    ADCON1=15;
    TRISD=0;
    TRISE=0;
    InicializaLCD();
    BorraLCD();
    MensajeLCD_Var("Hola mundo");
    while(1){
    }    
}
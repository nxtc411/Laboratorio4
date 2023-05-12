#include<xc.h>
#define _XTAL_FREQ 1000000
#include "LibLCDXC8.h"
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF

unsigned char Tecla = 0;
unsigned char i = 0;
unsigned char LeerTeclado(void);

void main(void){
    ADCON1=15;
    TRISD=0;
    TRISB=0b11110000;
    TRISE=0;
    LATB=0B00000000;
    RBPU=0;
    InicializaLCD();
    BorraLCD();
    MensajeLCD_Word("Hola mundo");
    while(1){
        while(RB4==1 && RB5==1 && RB6==1 && RB7==1);
        LATB=0b11111110;
        if(RB4==0) Tecla=1;
        else if(RB5==0) Tecla=2;
        else if(RB6==0) Tecla=3;
        else if(RB7==0) Tecla=4;
        else{
            LATB=0b11111101;
            if(RB4==0) Tecla=5;
            else if(RB5==0) Tecla=6;
            else if(RB6==0) Tecla=7;
            else if(RB7==0) Tecla=8;
            else{
                LATB=0b11111011;
                if(RB4==0) Tecla=9;
                else if(RB5==0) Tecla=10;
                else if(RB6==0) Tecla=11;
                else if(RB7==0) Tecla=12;
                else{
                    LATB=0b11110111;
                    if(RB4==0) Tecla=13;
                    else if(RB5==0) Tecla=14;
                    else if(RB6==0) Tecla=15;
                    else if(RB7==0) Tecla=16;
                    else Tecla=0;
                }
            }
        }
        
    }
}

unsigned char LeerTeclado(void){
    while(RB4==1 && RB5==1 && RB6==1 && RB7==1);//se bloquea al micro mientras no se oprima algún botón 
    LATB=0b11111110;
    if(RB4==0) return 1;
    else if(RB5==0) return 2;
    else if(RB6==0) return 3;
    else if(RB7==0) return 4;
    else{
    LATB=0b11111101;
    if(RB4==0) return 5;
    else if(RB5==0) return 6;
    else if(RB6==0) return 7;
    else if(RB7==0) return 8;
    else{
    LATB=0b11111011;
    if(RB4==0) return 9;
    else if(RB5==0) return 10;
    else if(RB6==0) return 11;
    else if(RB7==0) return 12;
    else{
    LATB=0b11110111;
    if(RB4==0) return 13;
    else if(RB5==0) return 14;
    else if(RB6==0) return 15;
    else if(RB7==0) return 16;
    }}}
}
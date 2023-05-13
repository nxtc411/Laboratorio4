#include<xc.h>
#define _XTAL_FREQ 1000000
#include "LibLCDXC8.h"
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF

unsigned char Tecla = 0;
unsigned long r = 0;
unsigned int n1 = 0;
unsigned int n2 = 0;
unsigned char op = ' ';
unsigned char n1c = ' ';
unsigned char n2c = ' ';
unsigned char LeerTeclado(void);

void main(void){
    ADCON1=15;
    TRISD=0;
    TRISB=0b11110000;
    TRISE=0;
    RBPU=0;
    InicializaLCD();
    BorraLCD();
    MensajeLCD_Word("Hola mundo");
    __delay_ms(2000);
    BorraLCD();
    while(1){
        LATB=0B00000000;
        Tecla = LeerTeclado();
        if(Tecla=='C'){
            DireccionaLCD(0xC0);
            MensajeLCD_Word("                ");
            DireccionaLCD(0xC0);
        }else{
            if(n1c==' '|n2c==' '|op==' '){
                if(op==' ' & (Tecla=='+'|Tecla=='-'|Tecla=='/'|Tecla=='x')){
                EscribeLCD_c(Tecla);
                op = Tecla;
                }else{
                    if(Tecla=='='){
                        if(n1c!=' ' & n2c!=' ' & op==' '){
                            EscribeLCD_c('=');
                            switch(op){
                            case '+': r = n1 + n2;
                                break;
                            case '-': r = n1 - n2;
                                break; 
                            case 'x': r = n1*n2;
                                break;
                            case '/': r = n1/n2;
                                break;
                            default:
                                r = 0;
                                break;
                            }         
                            EscribeLCD_c(r+'0');
                        }
                    }else{
                        if(n1c==' '){
                        EscribeLCD_c(Tecla);
                        n1c=Tecla;
                        n1 = Tecla-'0';
                    }else{
                        EscribeLCD_c(Tecla);
                        n2c=Tecla;
                        n2 = Tecla-'0';
                    }
                    }
                }
        }
        __delay_ms(700);
    }
    }}

unsigned char LeerTeclado(void){
    while(RB4==1 && RB5==1 && RB6==1 && RB7==1);//se bloquea al micro mientras no se oprima algún botón 
    LATB=0b11111110;
    if(RB4==0) return '1';
    else if(RB5==0) return '2';
    else if(RB6==0) return '3';
    else if(RB7==0) return '+';
    else{
    LATB=0b11111101;
    if(RB4==0) return '4';
    else if(RB5==0) return '5';
    else if(RB6==0) return '6';
    else if(RB7==0) return '-';
    else{
    LATB=0b11111011;
    if(RB4==0) return '7';
    else if(RB5==0) return '8';
    else if(RB6==0) return '9';
    else if(RB7==0) return '/';
    else{
    LATB=0b11110111;
    if(RB4==0) return 'C';
    else if(RB5==0) return '0';
    else if(RB6==0) return '=';
    else if(RB7==0) return 'x';
    }}}
    return '?';
}
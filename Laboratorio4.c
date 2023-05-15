#include<xc.h>
#define _XTAL_FREQ 1000000
#include "LibLCDXC8.h"
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF

unsigned char Tecla = 0;
unsigned int r = 0;
float d = 0;
unsigned int n1 = 0;
unsigned int n2 = 0;
unsigned int i = 0;
unsigned char op = ' ';
unsigned char n1c = ' ';
unsigned char n2c = ' ';
unsigned char LeerTeclado(void);
void Imprimir(unsigned char);
void Borrar(void);

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
    Borrar();
    while(1){
        LATB=0B00000000;
        Tecla = LeerTeclado();
        __delay_ms(200);
        Imprimir(Tecla);
    }}


void Borrar(void){
    DireccionaLCD(0x80);
    MensajeLCD_Word("                ");
    DireccionaLCD(0x80);
}

void Imprimir(unsigned char Valor){
    if(Valor=='C'){
            op = ' ';
            n1c = ' ';
            n2c = ' ';
            n1 = 0;
            n2 = 0;
            i = 0;
            Borrar();
        }else{
            if(n1c==' '|n2c==' '|op==' '){
                if(op==' ' & (Valor=='+'|Valor=='-'|Valor=='/'|Valor=='x')){
                DireccionaLCD(0x81);
                EscribeLCD_c(Valor);
                op = Valor;
                }else{
                    if(n1c==' '){
                        DireccionaLCD(0x80);  
                        EscribeLCD_c(Valor);
                        n1c=Valor;
                        n1 = Valor-'0';
                    }else{
                        if(n2c==' ' & op!=' '){
                            DireccionaLCD(0x82);
                            EscribeLCD_c(Valor);
                            n2c=Valor;
                            n2 = Valor-'0';
                        }
                    }
                    
                }
        }else{
           if(Valor=='='){
                        if(n1c!=' ' & n2c!=' ' & op!=' '){
                            DireccionaLCD(0x83);
                            EscribeLCD_c('=');
                            switch(op){
                            case '+': r = n1 + n2;
                                break;
                            case '-': r = n1 - n2;
                                break; 
                            case 'x': r = n1*n2;
                                break;
                            case '/': 
                                if(n2!=0){
                                    if(n1!=0){
                                    d = (float) n1/ (float) n2;
                                }
                                }else{
                                    if(n1 != 0 & n2 == 0) r = 1000;
                                    else if(n1 == 0 & n2 == 0) r = 1001;
                                }
                                break;
                            default:
                                r = 0;
                                break;
                            }
                            DireccionaLCD(0x84);
                            if(r>0x51 & r!=1000 & r !=1001){
                                EscribeLCD_c('-');
                                DireccionaLCD(0x85);
                                r = ~r+1;
                                EscribeLCD_c(r+'0'); 
                            }else{
                                if(r>0xA & r!=1000 & r !=1001){
                                    do{
                                        i = i+1;
                                        r = r - 10;
                                    }while(r>0xA);
                                    EscribeLCD_c(i+'0');
                                    DireccionaLCD(0x85);
                                    EscribeLCD_c(r+'0'); 
                                }else{
                                    if(r==1000){
                                            EscribeLCD_c('I');
                                            EscribeLCD_c('N');
                                            EscribeLCD_c('F');
                                    }else{
                                        if(r=1001){
                                            EscribeLCD_c('I');
                                            EscribeLCD_c('N');
                                            EscribeLCD_c('D');
                                        }else{
                                            if(d!=0) EscribeLCD_c(r+'0'); 
                                            else EscribeLCD_n8(d,2);
                                        }
                                    }
                                }
                            }
                        }
                    }     
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
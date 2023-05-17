#include<xc.h> //Libreria para PIC
#include<stdio.h> //Libreria adicional para manejo de decimales
#define _XTAL_FREQ 1000000 //Frecuencia de reloj
#include "LibLCDXC8.h" //Libreria para uso de LCD
#pragma config FOSC=INTOSC_EC //Sentencia para usar oscilador externo
#pragma config WDT=OFF //Apagar el perro guardian

unsigned char Tecla = 0; //Variable para leer teclado
unsigned long partdecl = 0; //Parte decimal de un resultado
unsigned long r = 0; //Resultado entero de las operaciones
unsigned int n1 = 0; //Primer numero
unsigned int n2 = 0; //Segundo numero
unsigned int i = 0;
unsigned long m = 0;
unsigned char op = ' '; //Operador
unsigned char n1c = ' '; //Primer numero en caracter
unsigned char n2c = ' ';  //Segundo numero en caracter
unsigned int color = 0;
unsigned char LeerTeclado(void); //Declarar funcion para lectura de matricial
void ColorRGB(void);

void main(void){
    ADCON1=15; //Deshabilitar funciones analogicas del puerto E
    TRISD=0; //Colocar puerto D como salida
    TRISB=0b11110000; //Colocar puerto B salida/entrada 50/50
    TRISE=0; //Colocar puerto E como salida
    TRISC=0; //Pines para RGB
    RBPU=0; //Activar resistencias pull up
    InicializaLCD(); //Funcion para configuracion inicial del LCD
    //Timer0 interrupcion
    T0CON=0b10000011;
    TMR0IF=0;
    TMR0=49911;
    TMR0IE=1;
    GIE=1;
    TMR0ON=1;
    //Fin de configuracion para Timer0
    BorraLCD(); //Limpiar el LCD
    MensajeLCD_Word("Hola mundo"); //Escribir mensaje de bienvenida
    __delay_ms(2000); //Retraso para evitar errores
    BorraLCD(); 
    while(1){
        LATB=0B00000000;
        Tecla = LeerTeclado();
        ColorRGB();
        if(Tecla=='C'){ //Limpiar la pantalla si se presiona [1][4] 
            DireccionaLCD(0xC0); //Colocar el cursor en la primera posicion de segunda fila
            //Limpiar variables
            op = ' ';
            n1c = ' ';
            n2c = ' ';
            n1 = 0;
            n2 = 0;
            i = 0;
            partdecl = 0;
            DireccionaLCD(0x80); //Colocar el cursor en la primera posicion de primera fila
            MensajeLCD_Word("                "); //Mandar mensaje vacio para limpiar
            DireccionaLCD(0xC0);
            DireccionaLCD(0x80);
        }else{
            if(n1c==' '|n2c==' '|op==' '){ //Funciones si no se ha recibido nada
                if(op==' ' & (Tecla=='+'|Tecla=='-'|Tecla=='/'|Tecla=='x') & n1c != ' ' & n2c ==' '){
                DireccionaLCD(0x81);
                EscribeLCD_c(Tecla);
                op = Tecla;
                }else if(n1c == ' ' & (Tecla!='+'|Tecla!='-'|Tecla!='/'|Tecla!='x'|Tecla!='='|Tecla!='C')){
                    DireccionaLCD(0x80);  
                    EscribeLCD_c(Tecla);
                    n1c=Tecla;
                    n1 = Tecla-'0';
                }else if(n2c==' ' & op!=' ' & n1c != ' ' & (Tecla!='+'|Tecla!='-'|Tecla!='/'|Tecla!='x'|Tecla!='='|Tecla!='C')){
                    DireccionaLCD(0x82);
                    EscribeLCD_c(Tecla);
                    n2c=Tecla;
                    n2 = Tecla-'0';
                }
            }else if(Tecla=='=' & n2c!=' ' & op!=' ' & n1c != ' '){
                DireccionaLCD(0x83);
                EscribeLCD_c('=');
                switch(op){
                case '+': 
                    r = n1 + n2;
                    partdecl = r*100;
                    break;
                case '-':
                    r = n1 - n2;
                    partdecl = r*100;
                    break; 
                case 'x': 
                    r = n1*n2;
                    partdecl = r*100;
                    break;
                case '/': 
                    if(n2!=0){
                        if(n1!=0){
                        r = n1/n2; //Parte entera
                        partdecl = (n1*100)/n2; //((n1/n2)-r)*10
                        }
                    }else{
                        if(n1 != 0 & n2 == 0) r = 1000;
                        else if(n1 == 0 & n2 == 0) r = 1001;
                        partdecl = r*100;
                    }
                    break;
                default:
                    r = 0;
                    break;
                }
                DireccionaLCD(0x84);
                if ((partdecl-(r*100))!= 0){
                    for (int i = 0; i < 3; i++) {
                        r = n1 / n2;
                        m = n1 % n2;
                        EscribeLCD_c(r+'0');
                        if(i == 0 && m != 0) {
                            EscribeLCD_c(',');
                        }
                        
                        if (m != 0) {
                            n1 = m * 10;
                        } else {
                            break;
                        }
                                              
                     }
                }else if(r>0x51 & r!=1000 & r !=1001){
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
                            if(r==1001){
                                EscribeLCD_c('I');
                                EscribeLCD_c('N');
                                EscribeLCD_c('D');
                            }else{
                                EscribeLCD_c(r+'0'); 
                            }
                        }
                    }
                }
            }else{
                EscribeLCD_c('W');
            }
        __delay_ms(700);
        }
    }
}

    
unsigned char LeerTeclado(void){
    while(RB4==1 && RB5==1 && RB6==1 && RB7==1);//se bloquea al micro mientras no se oprima alg?n bot?n 
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
    }
    }
    }
    return '?';
}

void ColorRGB(void){
    if(color==8) color = 0;
    switch(color){
        case 0:
            LATC = 0b00000000;
            break;
        case 1:
            LATC = 0b00000101;
            break;
        case 2:
            LATC = 0b00000110;
            break;
        case 3:
            LATC = 0b00000011;
            break;
        case 4:
            LATC = 0b00000010;
            break;
        case 5:
            LATC = 0b00000110;
            break;
        case 6:
            LATC = 0b00000100;
            break; 
        case 7:
            LATC = 0b00000111;
            break;   
        default:
            break;
    }
    color += 1;
}

void __interrupt() ISR(void){
    if(TMR0IF==1){
        TMR0IF=0;
        LATE2 = !LATE2;
        TMR0 = 49911;
    }
}

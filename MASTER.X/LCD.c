/*
 * File:   LCD.c
 * Author: PowerLab
 *
 * Created on 13 de febrero de 2020, 12:34 PM
 */
#define rs PORTBbits.RB1
#define E PORTBbits.RB0             //para no confundir los pines
#define data PORTD
#define _XTAL_FREQ 4000000
#include "LCD.h"
#include <xc.h>

void LCD8bits_init(void){           //configuracion inicial de la LCD
    __delay_ms(15);
    LCDcmd(0x030);
    __delay_ms(5);
    LCDcmd(0x030);
    __delay_ms(11);
    LCDcmd(0x030);
    __delay_ms(160);
    LCDcmd(0x38);
    LCDcmd(0x10);
    LCDcmd(0x01);
    LCDcmd(0x06);
    LCDcmd(0x0C);
    //LCDcmd(0x80);
}

void LCDcmd(unsigned char cmd){     //Escrinbe en la cmd de la LCD
    data = cmd;
    rs = 0;
    E = 1;
    __delay_ms(4);
    E = 0;
}

void LCDwrite(unsigned char dato){      //escribe un dato en la LCD
    data = dato;
    rs = 1;
    E = 1;
    __delay_ms(4);
    E = 0;
}

void LCDisplay(char *val){              //usa la anterior para escribir una cadena de caracteres
    while(*val){
        LCDwrite(*val++);
    }   
}

void LCDcursorSet(unsigned char row, unsigned char column){     //pone el cursor en la posicion que indique
    if (row == 1){
        LCDcmd(0x80 + column);
    } else if(row == 2){
        LCDcmd(0xC0 + column);
    }
}

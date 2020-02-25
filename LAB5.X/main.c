 /*
 * File:   main.c
 * Author: PowerLab
 *
 * Created on 13 de febrero de 2020, 12:33 PM
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>                 //libreria de variables
#include <stdio.h>                  //libreria para convertir numeros a caracteres
#define _XTAL_FREQ 4000000          //oscilador de 4MHz
#include <xc.h>

#include "LCD.h"  

void config(void);

void main(void) {
    config();               //configuracion de bits
    LCD8bits_init();        //inicio de la pantalla LCD
    
    return;
}

void config(void){
    TRISA = 0x03;           //ADCs
    TRISB = 0x00;           //LCD
    TRISC = 0x00;           //Comunicacion serial
    TRISD = 0x00;           //LCD
    ANSELH = 0x00;
    ANSEL = 0x00;
    ANSELbits.ANS0 = 1;     
    ANSELbits.ANS1 = 1;
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
}

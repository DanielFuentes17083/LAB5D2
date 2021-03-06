 /*
 * File:   main.c
 * Author: PowerLab
 *
 * Created on 13 de febrero de 2020, 12:33 PM
  * 
  * La libreria e interrupciones utilizadas para la comunicacion I2C fueron tomadas del ejemplo de Pablo Mazariegos subido en clase
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

#include <stdint.h>                 //libreria de variables
#include <pic16f887.h>
#define _XTAL_FREQ 8000000      //Definicion de frecuencia
#include <xc.h>

#include "I2C.h"

uint8_t z;
uint8_t dato;
uint8_t readed;
uint8_t cont = 0;
uint8_t past1 = 0;
uint8_t past2 = 0;

void setup(void);

void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 
        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            readed = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = PORTA;             //Envia el contador
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}

void main(void) {
    setup();
    while(1){
        if (PORTBbits.RB0 == 1 && past1 == 0){                                  //si presiona P1 para moverse y guarda valor
            if(PORTA == 15){
                PORTA = 0;
            }else{
                PORTA++;
            }
            past1 = 1;
        }
        if (PORTBbits.RB1 == 1 && past2 == 0){                                  //si presiona P2 para moverse y guarda valor
            PORTA--;
            if(PORTA > 15){
                PORTA = 15;
            }
            past2 = 1;
        }
        if (PORTBbits.RB0 == 0){                                                
            past1 = 0;
        }
                                                                                //antirrebote
        if (PORTBbits.RB1 == 0){
            past2 = 0;
        }
    }
    return;
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;                  //LEDs
    TRISB = 0;                  //Botones
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    PORTA = 0;
    I2C_Slave_Init(0x40);       //Configura la comunicacion I2C como Slave con direccion 0x40
}
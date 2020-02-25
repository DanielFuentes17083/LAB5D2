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

#include <stdint.h>                 //libreria de variables
#include <stdio.h>                  //libreria para convertir numeros a caracteres
#define _XTAL_FREQ 4000000          //oscilador de 4MHz
#include <xc.h>

#include "LCD.h"  
#include "I2C.h"

void config(void);

uint8_t cont = 0;
uint8_t va1 = 0;
float pot1 = 0;
int int1 = 0;
int dec1 = 0;
float dectemp1 = 0;
char contStr[1];
char potStr1[3];
char decpot1[2];

void main(void) {
    config();               //configuracion de bits
    LCD8bits_init();        //inicio de la pantalla LCD
    LCDcursorSet(1,1);
    LCDisplay("S1:");
    LCDcursorSet(1,6);
    LCDisplay("CONT:");               //Pone los titulos en la pantalla
    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        va1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        sprintf(contStr, "%d",cont);
        
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        cont = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        pot1 = va1;
        pot1 = pot1 * 5/255;
        int1 = pot1;
        dectemp1 = (pot1 - int1)*100;   //obtiene el entero y los decimales por aparte del primer pot
        dec1 = dectemp1;
        sprintf(potStr1, "%d",int1);
        sprintf(decpot1, "%d",dec1);
        
        LCDcursorSet(2,0);
        LCDisplay(potStr1);         
        LCDisplay(".");
        if (dec1<10){
            LCDisplay("0");
            LCDisplay(decpot1);         //Despliegue del primer valor
        }else{
            LCDisplay(decpot1);
        } 
        LCDisplay("V");
        LCDcursorSet(2,6);
        if (cont <= 9){
            LCDisplay("0");
            LCDisplay(contStr);
        }else{
            LCDisplay(contStr);
        }
        
        
        
    }
    
    return;
}

void config(void){
    TRISB = 0x00;           //LCD
    TRISD = 0x00;           //LCD
    ANSELH = 0x00;
    ANSEL = 0x00;
    ANSELbits.ANS0 = 1;     
    ANSELbits.ANS1 = 1;
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}

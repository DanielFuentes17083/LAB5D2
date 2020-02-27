 /*
 * File:   main.c
 * Author: PowerLab
 *
 * Created on 13 de febrero de 2020, 12:33 PM
 */

/*
 * El codigo utilizado para la comunicacion I2C entre el master y el RTC fue creado por
 * Aswinth Raj; fue tomado del sitio https://circuitdigest.com/microcontroller-projects/pic16f877a-ds3231-rtc-digital-clock
 * y editado por Daniel Fuentes Oajaca
 * 
 * La libreria de la comunicación I2C fue tomada del ejemplo subido por Pablo Mazariegos
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

#include "LCD.h"                    //Headers
#include "I2C.h"

void config(void);                  
int  BCD_2_DEC(int to_convert);     //prototipos de funciones
int DEC_2_BCD (int to_convert);

uint8_t cont = 0;                   //Variables
uint8_t va1 = 0;
uint8_t sec = 00;
uint8_t min = 00;
float pot1 = 0;
int int1 = 0;
int dec1 = 0;
float dectemp1 = 0;
char contStr[1];
char potStr1[3];
char decpot1[2];
char sec1[2];
char min1[2];

void main(void) {
    config();               //configuracion de bits
    LCD8bits_init();        //inicio de la pantalla LCD
    LCDcursorSet(1,1);      
    __delay_ms(2);
    LCDisplay("S1:");
    LCDcursorSet(1,5);
    LCDisplay("CONT:");               //Pone los titulos en la pantalla
    LCDcursorSet(1,11);
    LCDisplay("TIME:");
    
    while(1){
        
        I2C_Master_Start();                                  //Llama al RTC y le pide los datos   
        I2C_Master_Write(0xD0);                         
        I2C_Master_Write(0);    
        I2C_Master_Stop();

        I2C_Master_Start();
        I2C_Master_Write(0xD1);                              //Llama al RTC y le pide los segundos
        sec = BCD_2_DEC(I2C_Master_Read(0));                 
        I2C_Master_Stop(); 

        I2C_Master_Start();
        I2C_Master_Write(0xD1);                              //Llama al RTC y le pide los minutos
        min = BCD_2_DEC(I2C_Master_Read(0));                
        I2C_Master_Stop();  

      //END Reading  
        I2C_Master_Start();
        I2C_Master_Write(0xD1);                              
        I2C_Master_Read(0);                                 
        I2C_Master_Stop();
        __delay_ms(10);
        
        sprintf(sec1, "%d",sec);                            //convierte los valores de minutos y segundos a strings
        sprintf(min1, "%d",min);
        
        
        I2C_Master_Start();                                 //Llama al pic con la direccion 0x50 (ADC) y lo lee
        I2C_Master_Write(0x51);
        va1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        
        I2C_Master_Start();                                 //Llama al pic con la direccion 0x40 (contador) y lo lee
        I2C_Master_Write(0x41);
        cont = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        
        sprintf(contStr, "%d",cont);                        //convierte el valor obtenido del contador en string
        
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
        
        LCDcursorSet(2,7);
        if (cont <= 9){
            LCDisplay("0");
            LCDisplay(contStr);         //Despliegue del contador en la LCD
        }else{
            LCDisplay(contStr);
        }
        
        
        LCDcursorSet(2,11);
        if(min < 10){
            LCDisplay("0");
            LCDisplay(min1);
        }else{
            LCDisplay(min1);
        }
        LCDcursorSet(2,13);             //Despliegue del RTC en la LCD
        LCDisplay(":");
        if(sec < 10){
            LCDisplay("0");
            LCDisplay(sec1);
        }else{
            LCDisplay(sec1);
        }
        
        
    }
    
    return;
}

void config(void){
    TRISB = 0x00;           //LCD
    TRISD = 0x00;           //LCD
    ANSELH = 0x00;
    ANSEL = 0x00;
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}

int  BCD_2_DEC(int to_convert)      //Convierte del formato BCD (como lo envia el RTC al pic) a un entero
{
   return (to_convert >> 4) * 10 + (to_convert & 0x0F);
}

int DEC_2_BCD (int to_convert)      //Convierte un entero a formato BCD
{
   return ((to_convert / 10) << 4) + (to_convert % 10);
}
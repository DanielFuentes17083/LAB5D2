
#ifndef __LCD_H
#define	__LCD_H

#include <xc.h> // include processor files - each processor file is guarded.  
void LCD8bits_init(void);
void LCDcmd(unsigned char cmd);
void LCDwrite(unsigned char dato);
void LCDisplay(char *val);
void LCDcursorSet(unsigned char row, unsigned char column);


#endif	/* XC_HEADER_TEMPLATE_H */


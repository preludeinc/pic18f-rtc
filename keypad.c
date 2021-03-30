/* 
 * File:   keypad.c
 */

#include <xc.h>
#include "lcd18f.h"
#include "keypad.h"
#include "clockcode.h"

#pragma config OSC = HS, FCMEN = OFF, PWRT = ON, BOREN = OFF
#pragma config WDT = OFF, MCLRE = ON, PBADEN = OFF, LVP = OFF

#define _XTAL_FREQ 20000000 

// function prototype
unsigned char keypad(void);

// switch case function which checks if matrix keys are pressed
unsigned char keypad(void)
{	 
  __delay_ms(30);                    // switch de-bounce delay 
	
  Nop();
	Nop();
    
	LATBbits.LATB0 = 0;          // 0 is rotated through lower nibble
	LATBbits.LATB1 = 1;                 
	LATBbits.LATB2 = 1;          // internal weak pull-ups - checks which is low
	LATBbits.LATB3 = 1;
	Nop();
	Nop();
	
  // checking the first row of the keypad	
  if(!PORTBbits.RB4)return('1');      
	if(!PORTBbits.RB5)return('4');
	if(!PORTBbits.RB6)return('7');
	if(!PORTBbits.RB7)return('*');
	LATBbits.LATB0 = 1;
	LATBbits.LATB1 = 0;
	Nop();
	Nop();
	
  // checking the second row of the keypad
  if(!PORTBbits.RB4)return('2');     
	if(!PORTBbits.RB5)return('5');
	if(!PORTBbits.RB6)return('8');
	if(!PORTBbits.RB7)return('0');
	LATBbits.LATB1 = 1;
	LATBbits.LATB2 = 0;
	Nop();
	Nop();
	
  // checking the third row of the keypad	
  if(!PORTBbits.RB4)return('3');      
	if(!PORTBbits.RB5)return('6');	
	if(!PORTBbits.RB6)return('9');
	if(!PORTBbits.RB7)return('#');
	LATBbits.LATB2 = 1;
	LATBbits.LATB3 = 0;
	Nop();
	Nop();
    
    // checking the fourth row of the keypad
    if(!PORTBbits.RB4)return('A');    
	if(!PORTBbits.RB5)return('B');
	if(!PORTBbits.RB6)return('C');
	if(!PORTBbits.RB7)return('D');

	return(0);                         // no valid key pressed, exit switch case
}


/**************************************************

Description:          A real-time clock made using interrupts, and is displayed on a Hitachi LCD. 
                      The user is prompted to enter a chosen time using the matrix 4x4 keypad.

**************************************************/

#include  <xc.h> 
#include "lcd18f.h"
#include "keypad.h"
#include "clock_code.h"

#pragma config OSC = HS, FCMEN = OFF, PWRT = ON, BOREN = OFF
#pragma config WDT = OFF, MCLRE = ON, PBADEN = OFF, LVP = OFF

#define _XTAL_FREQ 20000000 

// prototypes for interrupt and clock functions
void __interrupt() tickINT(void);   
void update_time(char time[], unsigned char *hours, unsigned char *minutes, unsigned char *seconds, unsigned char *is_am);
void display_time(char time[], unsigned char *hours, unsigned char *minutes, unsigned char *seconds, unsigned char *is_am);
void set_time(unsigned char key, unsigned char *hours, unsigned char *minutes, unsigned char *seconds, unsigned char *is_am);
 
unsigned char ticks = 0;                                          // global ticks variable to increment interrupt count 

void main(void)
{ 
    TRISA = 0xFF;                                                 // port A set to an output
    TRISB = 0xF0;                                                 // upper nibble inputs lower outputs for keypad
    TRISC = 0x00;                                                 // output for the LED bar
    TRISD = 0;                                                    // output for the LCD
    
    ADCON1 = 0x0F;                                                // AN1 - AN12 are digital inputs
    CMCON = 0x07;                                                 // comparators are turned off
    
    INTCON2bits.nRBPU = 0;                                        // weak pull-ups turned on for port B
    
    lcd_init();                                                   // configures external LCD 
    rtc_init();
    lcd_bl(1);                                                    // back light is on
    lcd_clear();                                                  // clears LCD
                   
    unsigned char key;                                            // holds keypad state 
    char time[] = "12:00:00 xm       ";                           // initializes time to 12:00 am 
    unsigned char seconds = 59;                                   // time variables
    unsigned char minutes = 59;
    unsigned char hours = 11;
    unsigned char is_am = 0;                                      // AM flag
    
    lcd_puts_const(" Current Time:");                             // prints a prompt to the LCD for the user
    
    while (1)
    {  
        if(ticks > 99)                                            // if ticks is greater than 99, roll-over
        {
            ticks -=100;                                          // increment time
            update_time(time, &hours, &minutes, &seconds, &is_am);
            display_time(time, &hours, &minutes, &seconds, &is_am);
        } // end if
        
            key = keypad();                                      // when key pressed, call keypad function
            if (key !=0)                                         // if a key is pressed user can set time
            {                                                    
            set_time(key, &hours, &minutes, &seconds, &is_am);
            display_time(time, &hours, &minutes, &seconds, &is_am);
            }                                                    // switch de-bounce
            while (keypad()!=0)                                  // waits for key to be released 
            {
            }
    } // end while
    return;
} // end main

void update_time(char time[], unsigned char *hours, unsigned char *minutes, unsigned char *seconds, unsigned char *is_am)
{
        if (++*seconds > 59)                                    // increment seconds - if seconds > 59, increment minutes  
        {
        *seconds -= 60;                                         // checks for accuracy of roll-over
        *minutes += 1;                                          // minutes rolls over to 1
            
            if(*minutes > 59)                                   // if minutes > 59, increment hours
            {
                *minutes -= 60;
                *hours += 1;                                    // hours rolls over to 1
                
                if (*hours > 12)                                // if hours > 12, increment hours (hours rolls over to 1)
                {
                    *hours -= 12;
                    *hours = 1;
                }
                else if (*hours == 12)                          // toggle am_flag if hours is equal to 12
                {       
                    *is_am ^=1;                                 // toggles AM flag
                } 
            }
        }          
} // end update_time()

void display_time(char time[], unsigned char *hours, unsigned char *minutes, unsigned char *seconds, unsigned char *is_am)
{
        time[0] = (*hours / 10) + '0';                          // first hours digit, converted to ASCII 
        time[1] = (*hours % 10) + '0';                          // takes remainder of hours to display second digit
        
        time[3] = (*minutes / 10) + '0';                        // minutes MSD
        time[4] = (*minutes % 10) + '0';                        // minutes LSD
        
        time[6] = (*seconds / 10) + '0';                        // seconds MSD
        time[7] = (*seconds % 10) + '0';                        // seconds LSD
        time[9] = *is_am ? 'a' : 'p';                           // ternary operator - is AM flag true?
        
        lcd_goto(0x42);                                         
        for (int i = 0; i < 11; i++)                            // prints time to LCD
        {
        lcd_write(time[i]);
        }   
} // end display_time())

// user entered setTime function (called on keypad key-press)
void set_time(unsigned char key, unsigned char *hours, unsigned char *minutes, unsigned char *seconds, unsigned char *is_am)
{
        switch (key)
            { 
                case 'A':                                          // when A is pressed, hours increments
                    *hours += 1;
                    if (*hours > 12)                               // hours resets to 1 after it reaches 12
                    {
                        *hours = 1;
                        *is_am ^=1;
                    }
                break; 
                case 'B':                                         // when B is pressed, minutes increments
                    *minutes += 1;
                    if (*minutes > 59)                            // minutes resets to 0 after it reaches 59
                    {
                        *minutes = 0;
                        *hours += 1;                              // hours increments when minutes rolls over
                    }
                break;   
                case 'C':                                         // when C is pressed, seconds increments
                    *seconds += 1;
                    if(*seconds > 59)                             // seconds resets to 0 after it reaches 59
                    {
                        *seconds = 0;
                        *minutes += 1;                            // minutes increments when seconds rolls over
                    }
                break;
                case 'D':                                         // if 'D' is pressed, time resets to 12:00 AM
                    *hours = 12;
                    *minutes = 0;
                    *seconds = 0;
                    *is_am = 0;                     
                break;
                case '*': break;                                 // nothing is printed in below empty cases
                case '#': break;
                default:  break;                                 // if numbers are pressed, they are printed to LCD
                } // end switch   
                while (keypad()!=0)                              // key de-bounce
                {
                }
} // end setTime()  

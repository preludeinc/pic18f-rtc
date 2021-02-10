/*
 * File:   clock_code.h
 */

// global ticks variable for clock function
extern unsigned char ticks;

// real-time clock initialization function 
extern void rtc_init(void);

// interrupt prototype
void __interrupt() tickINT(void); 

/* clock function */
extern void update_time(char time[], unsigned char *hours, unsigned char *minutes, unsigned char *seconds, unsigned char *is_am);

extern void display_time(char time[], unsigned char *hours, unsigned char *minutes, unsigned char *seconds, unsigned char *is_am);
// user entered time function
extern void set_time(unsigned char key, unsigned char *hours, unsigned char *minutes, unsigned char *seconds, unsigned char *is_am);

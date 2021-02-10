/*
 * File: lcd18f.h 
 */

/* LCD Functions */

/* enable or disable LCD back light */
extern void lcd_bl(unsigned char c);

/* writes a byte to the LCD in 4 bit mode */
extern void lcd_write(unsigned char b);

/* initializes the LCD */
extern void lcd_init(void);

/* clears and homes the LCD */
extern void lcd_clear(void);

/* writes a string of characters to the LCD */
extern void lcd_puts(unsigned char *s);

extern void lcd_puts_const(const char *s);

/* writes a single character to the LCD */
extern void lcd_putch(unsigned char);

/* go to specified position */
extern void lcd_goto(unsigned char pos);

extern void lcd_cmd(unsigned char c);

/* sets the cursor position, takes F, masks it, and puts a 1 in the MSB */
#define lcd_Set_Cursor(x) lcd_write((x)&0x7f|0x80)  




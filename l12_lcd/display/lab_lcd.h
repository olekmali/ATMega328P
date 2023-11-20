//* lab_lcd.h *
#ifndef LAB_LCD_H_
#define LAB_LCD_H_

#include <stdint.h>

/* Lab 12 */
//void  lcd_put_nibble(uint8_t mode, uint8_t data) - library internal
void    lcd_init();

/* Homework 12 */
void    lcd_putchr(char   chr);
void    lcd_putstr(const char * str);

void    lcd_putcmd(uint8_t cmnd);
void    lcd_clear (void);       // clears display and returns to 0 position
void    lcd_home  (void);       // returns to 0 position
void    lcd_cursor(uint8_t on); // show cursor: 0=off , 1=on
void    lcd_gotoXY(uint8_t r, uint8_t c); // optional feature

/* Extra Credit 12 */
//uint8_t lcd_get_nibble(uint8_t mode) - library internal
char    lcd_getchr();
uint8_t lcd_getcmd();           // returns the LCD status byte
void    lcd_wait (void);


#endif /* LAB_LCD_H_ */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hardware/uart.h"
#include "console.h"

void console_putc(char c) { uart_putc(UART_ID, c); printf("%c", c); }

char console_getc(void) { 
  uint8_t ch;

  //ch = uart_getc(UART_ID);
  ch = getchar();

  return (char) ch;
} 

void console_puts(char *s) {
  // put string to console output
  while (*s != '\0') {

    if (*s == '\r') {
      console_putc('\n');
    } else {
      console_putc(*s);
    } // end if

    s++;
  } // end while
} // end console_puts

int console_gets(char *s, int len) {
  char *t = s;
  char c, cn;
  uint8_t up_arrow_flag = 0;

  *t = '\000';

  // read until a <LF> is received
  while ((c = console_getc()) != '\r') {
    switch (c) {
    case 27:
      cn = console_getc();
      if (cn == '[') { // pressed up arrow
        cn = console_getc();
        if (cn == 'A') {
          console_putc(0x08);
          console_putc(' ');
          console_putc(0x08);
          up_arrow_flag = 1;
        } // end if
      }   // end if
      break;
	  
    case 0x08: 					// backspace
    case 0x7F: 					// delete
      console_putc(0x08);
      console_putc(' ');
      console_putc(0x08);
      for (int i = 0; i < 2; i++)
        t--;
      break;
    case '\r': 					// linefeed
      *t = c;
      console_putc(c);

    case 32 ... 126: 			// only valid ascii characters
      *t = c;
      console_putc(c);
      break;
    default:
      t--;
    } // end switch

    if (up_arrow_flag == 1)
      break;

    if ((t - s) < len) {
      t++;
    } // end if

    // update end of string with NUL
    *t = '\000';

  } // end while

  if (up_arrow_flag == 1) {
    return -1;
  } else {
    return t - s;
  } // end if
}


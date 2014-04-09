/*******************************************************************************
 *        
 * console.c
 *
 * Wiktor Lukasik
 *
 * VGA driver to output text to the screen. Assumes 80x25 console.
 *
 ******************************************************************************/

#include "console.h"
#include "io.h"

/* Current linear position in video buffer */
int console_position = 0;
/* Pointer to VGA buffer */
unsigned char * videoram = (unsigned char *) 0xb8000;

/* Redraws cursor at console_position. */
void console_update_cursor() {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(console_position&0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char )((console_position>>8)&0xFF));
}

/* Clear screen, works by filling VGA buffer with 0s. */
void console_cls() {
    RESET_CONSOLE_POSITION;
    int i = 0;
    while( i < 2 * CONSOLE_LENGTH ) {
        videoram[i++] = 0;
        videoram[i++] = 0x24;
    }

}

/* Prints string. Works by calling printchr for each char in the string. */
void console_print(char * str) {
    char * ptr = str;
    while( *ptr != 0 ) {
        char c = *ptr;
        console_printchr(c);
        ptr++;
    }
}

/* Print character. 
   '\n' char is interpreted as new line.
   Screen is scrolled up once buffer is filled. */
void console_printchr(char c) {

    /* Check for end of buffer. */
    if( console_position == CONSOLE_ROWS * CONSOLE_COLS ) {
        int i;

        /* Move all lines one line up. */
        for(i = 0; i < CONSOLE_LENGTH * 2; i += 2) {
            videoram[i] = videoram[i + 2 * CONSOLE_COLS]; 
            videoram[i + 1] = videoram[i + 2 * CONSOLE_COLS + 1]; 
        }
        
        /* Clear the last line as there might have been garbage. */
        for( console_position = (CONSOLE_ROWS - 1) * CONSOLE_COLS; console_position < CONSOLE_ROWS * CONSOLE_COLS; console_position++) {
            videoram[GET_CONSOLE_CHAR_POS] = ' ';
            videoram[GET_CONSOLE_MODE_POS] = 0x24;

        }

        console_position = (CONSOLE_ROWS - 1) * CONSOLE_COLS;
    }

    /* If char is new line, just move to beginning of the next line.
       Otherwise, output char. */
    if( c == '\n' ) {
        long row = console_position / CONSOLE_COLS;
        row++;
        console_position = row * CONSOLE_COLS;
    } else {
        videoram[GET_CONSOLE_CHAR_POS] = c;
        videoram[GET_CONSOLE_MODE_POS] = 0x24;
        console_position++;
    }

    /* Update cursor once char is printed and current position adjusted. */
    console_update_cursor();
}



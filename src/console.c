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
        videoram[i++] = CONSOLE_COLOR;
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
            videoram[GET_CONSOLE_MODE_POS] = CONSOLE_COLOR;

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
        videoram[GET_CONSOLE_MODE_POS] = CONSOLE_COLOR;
        console_position++;
    }

    /* Update cursor once char is printed and current position adjusted. */
    console_update_cursor();
}

int console_dignum(int i) {
    if(i < 10) {
        return 1;
    } else if(i < 100) {
        return 2;
    } else if(i < 1000) {
        return 3;
    } else {
        int c = 0;
        while(i > 0) {
            //console_printchr('C');
            i = i / 10;
            c++;
        }
        console_printchr(c + 48);
        return c;
    }
}

/* TODO: fix */
void console_printnum(int i) {
    int o = i;
    int j;
    int l;
    int m = 1;
    int c;

    c = 0;
    while(i > 0) {
        i = i / 10;
        c++;
    }
    l = c;
    

    for(j = 1; j < l; j++) {
        m *= 10;
    }
    
    do {
        j = o / m;
        console_printchr(j+48);
        o = o % m;
        m = m / 10;
    } while(m > 0);
}

void console_printhex(int i) {
    char buf[8];
    char * template = "0123456789ABCDEF";
    int j = i;
    int c = 0;

    console_print("0x");
    do {
        buf[c++] = template[j % 16];
        j /= 16;
    } while(j > 0);

    while(c--) {
        console_printchr(buf[c]);
    }
}

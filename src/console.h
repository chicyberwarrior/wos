#ifndef console_h
#define console_h

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHT_GRAY 7
#define COLOR_DARK_GRAY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 0xA
#define COLOR_LIGHT_CYAN 0xB
#define COLOR_LIGHT_RED 0xC
#define COLOR_LIGHT_MAGENTA 0xD
#define COLOR_YELLOW 0xE
#define COLOR_WHITE 0xF

#define CONSOLE_COLOR (COLOR_BLACK << 4) | (COLOR_WHITE & 0xF)

#define CONSOLE_COLS 80
#define CONSOLE_ROWS 25
#define CONSOLE_LENGTH CONSOLE_ROWS * CONSOLE_COLS
#define GET_CONSOLE_CHAR_POS console_position * 2                                                                                                             
#define GET_CONSOLE_MODE_POS GET_CONSOLE_CHAR_POS + 1                                                                                                         
#define GET_CONSOLE_CUR_ROW console_position / CONSOLE_COLUMNS                                                                                                
#define GET_CONSOLE_CUR_COL console_position % CONSOLE_COLUMNS
#define RESET_CONSOLE_POSITION console_position = 0
void console_update_cursor();
void console_cls();
void console_printchr(char c);
void console_print(char * str);
void console_printnum(int i);
void console_printhex(int i);

#endif

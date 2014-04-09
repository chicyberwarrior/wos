#ifndef console_h
#define console_h

//#define CONSOLE_MEMADDR 0xB8000
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

#endif

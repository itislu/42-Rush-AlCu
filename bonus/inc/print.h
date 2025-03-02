#pragma once

#include "alcu.h"
#include <stddef.h>

#define PRT_PIECE       "📍"
#define PRT_ROW_LIMIT   16 // how many rows/heaps to show
#define PRT_WIDTH_LIMIT 38 // how many 📍 to show
#define PRT_OFFSET      5  // how many lines to go up
#define PRT_PIN_FILLER  15 // fillers left and right of ... XX 📍 ...

/* clear */
void clear_rows(int rows);
void clear_line(void);
void move_down_a_line(void);
void move_up_a_line(void);

/* print_board */
void print_board_gameloop(t_board *board, unsigned int pieces);
void print_board_complete(t_board *board);

/* print_boxed */
void print_boxed(const char *str);
void print_boxed_specialstr(const char *str, size_t str_len);

/* print_game */
void print_game_mode(Mode mode);
void print_result(t_board *board);

#pragma once

#include <stdbool.h>
#include <stddef.h>

#define MAX_ROWS       42
#define MIN_ROW_AMOUNT 1
#define MAX_ROW_AMOUNT 10000
#define MIN_PICKS      1
#define MAX_PICKS      3

// --- PRINT stuff
#define TITLE \
	"\
              ╭───────────────────────╮\n\
              │  ____ ____ ____ ____  │\n\
              │ ||A |||l |||C |||u || │\n\
              │ ||__|||__|||__|||__|| │\n\
              │ |/__\\|/__\\|/__\\|/__\\| │\n\
              │                       │\n\
              ╰───────────────────────╯\n\
"

typedef enum Result {
	RESULT_OK,
	USAGE_ERROR,
	BOARD_ERROR,
	INTERNAL_ERROR,
	USER_EXIT,
	SIZE_ERROR
} Result;

typedef enum Mode {
	LAST_LOSES = 1,
	LAST_WINS
} Mode;

typedef enum Player {
	AI = -1,
	PLAYER = 1
} Player;

typedef struct s_row {
	unsigned int start_amount;
	unsigned int cur_amount;
	Player pref_finisher;
	Player last_pick;
} t_row;

typedef struct s_board {
	t_row **rows;
	size_t height;
	unsigned int width;
	size_t cur_row;
	Mode game_mode;
	unsigned int *picks;
	size_t cur_turn;
	unsigned int num_options;
} t_board;

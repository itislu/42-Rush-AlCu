#pragma once

#include <stdbool.h>
#include <stddef.h>

#define MAX_ROWS 42

typedef enum Result {
	OK,
	USAGE_ERROR,
	BOARD_ERROR,
	INTERNAL_ERROR
} Result;

typedef enum Mode {
	LAST_WINS,
	LAST_LOSES
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
	t_row *rows;
	size_t cur_row;
	unsigned int width;
	size_t height;
	Mode game_mode;
} t_board;

// If filename == NULL, read from stdin
Result init_board(t_board *board, const char *filename);

void free_board(t_board *board);

unsigned int prompt(t_board *board);
bool is_valid_number(const char *num);

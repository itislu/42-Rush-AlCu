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

typedef enum Player {
	AI,
	PLAYER
} Player;

typedef struct s_row {
	unsigned int start_amount;
	unsigned int amount;
	Player pref_finisher;
} t_row;

typedef struct s_board {
	t_row *rows;
	size_t cur_row;
	unsigned int width;
} t_board;

// If filename == NULL, read from stdin
Result init_board(t_board *board, const char *filename);

void free_board(t_board *board);

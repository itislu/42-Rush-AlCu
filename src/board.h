#pragma once

#include <stdbool.h>
#include <stddef.h>

#define MAX_ROWS       42
#define MIN_ROW_AMOUNT 1
#define MAX_ROW_AMOUNT 10000
#define MIN_PICKS      1
#define MAX_PICKS      3

// --- PRINT stuff
#define PIECES          "📍"
#define INPUT_OFFSET    3
#define PRT_ROW_LIMIT   10
#define PRT_WIDTH_LIMIT 26
#define AI_OFFSET		3
#define PLAYER_OFFSET	2
#define PRT_PIN_FILLER	3

typedef enum Result {
	OK,
	USAGE_ERROR,
	BOARD_ERROR,
	INTERNAL_ERROR,
	USER_EXIT
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
} t_board;

// If filename == NULL, read from stdin
Result init_board(t_board *board, const char *filename);
void free_board(t_board *board);

Result prompt_picks(t_board *board, unsigned int *picks);
Result prompt_game_mode(Mode *mode);
bool is_valid_number(const char *num, unsigned int lower, unsigned int upper);
Result get_input(char **line);

void print_board_gameloop(t_board *board, unsigned int pieces);
void print_board_complete(t_board *board);

void clear_rows(int rows);

#include "board.h"
#include <stddef.h>

static bool read_board(t_board *board, int fd);

// If filename == NULL, read from stdin
Result init_board(t_board *board, const char *filename)
{
	(void)board;
	(void)filename;

	// read_board()

	// prompt_game_mode()

	return true;
}

static bool read_board(t_board *board, int fd)
{
	(void)board;
	(void)fd;
	return true;
}

void free_board(t_board *board)
{
	(void)board;
}

void calc_finishers(t_board *board)
{
	for (size_t row = 1; row < board->height; row++) {
		t_row *prev_row = &board->rows[row - 1];
		t_row *cur_row = &board->rows[row];
		Player prev_finisher = prev_row->pref_finisher;

		if (prev_finisher == AI) {
			cur_row->pref_finisher =
			    (cur_row->start_amount % 4 == 0) ? PLAYER : AI;
		}
		else {
			cur_row->pref_finisher =
			    (cur_row->start_amount % 4 == 1) ? PLAYER : AI;
		}
	}
}

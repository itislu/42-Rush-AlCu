#include "init.h"
#include "board.h"
#include "libft.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static Result read_board(t_board *board, const char *filename);
static void calc_finishers(t_board *board);

// If filename == NULL, read from stdin
Result init_board(t_board *board, const char *filename)
{
	Result res = read_board(board, filename);
	if (res != OK) {
		return res;
	}

	board->cur_row = (board->height != 0) ? board->height - 1 : 0;

	res = prompt_game_mode(&board->game_mode);
	if (res == OK) {
		calc_finishers(board);
	}
	return res;
}

void free_board(t_board *board)
{
	size_t i = 0;

	while (board->rows[i] != NULL) {
		free(board->rows[i]);
	}
	ft_free_and_null((void **)&board->rows);
}

static Result read_board(t_board *board, const char *filename)
{
	t_list *rows = NULL;
	Result res = read_rows(&rows, filename);

	if (res == OK) {
		board->height = ft_lstsize(rows);
		board->rows = (t_row **)ft_lstto_array(&rows);
		if (board->rows == NULL) {
			res = INTERNAL_ERROR;
		}
	}
	ft_lstclear(&rows, free);
	return res;
}

static void calc_finishers(t_board *board)
{
	for (size_t row = 1; row < board->height; row++) {
		t_row *prev_row = board->rows[row - 1];
		t_row *cur_row = board->rows[row];
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


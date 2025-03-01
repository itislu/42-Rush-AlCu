#include "init.h"
#include "board.h"
#include "get_next_line.h"
#include "libft.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/param.h>
#include <unistd.h>

static Result read_board(t_board *board, const char *filename);
static unsigned int max_width(t_board *board);
static void calc_finishers(t_board *board);

// If filename == NULL, read from stdin
Result init_board(t_board *board, const char *filename)
{
	ft_bzero(board, sizeof(*board));
	Result res = read_board(board, filename);
	if (res != OK) {
		return res;
	}

	board->width = max_width(board);
	board->cur_row = board->height - 1;

	res = prompt_game_mode(&board->game_mode);
	if (res == OK) {
		calc_finishers(board);
	}
	return res;
}

void free_board(t_board *board)
{
	if (board->rows != NULL) {
		for (size_t i = 0; board->rows[i] != NULL; i++) {
			free(board->rows[i]);
		}
		ft_free_and_null((void **)&board->rows);
	}
	free_get_next_line();
}

static Result read_board(t_board *board, const char *filename)
{
	t_list *rows = NULL;
	Result res = read_rows(&rows, filename);

	if (res == OK) {
		board->height = ft_lstsize(rows);
		board->rows = (t_row **)ft_lstto_array(&rows);
		if (board->height == 0) {
			res = BOARD_ERROR;
		}
		else if (board->rows == NULL) {
			res = INTERNAL_ERROR;
		}
	}
	ft_lstclear(&rows, free);
	return res;
}

static unsigned int max_width(t_board *board)
{
	unsigned int max = 0;

	for (size_t row = 0; row < board->height; row++) {
		max = MAX(max, board->rows[row]->start_amount);
	}
	return max;
}

static void calc_finishers(t_board *board)
{
	board->rows[0]->pref_finisher =
	    (board->game_mode == LAST_LOSES) ? PLAYER : AI;

	for (size_t row = 1; row < board->height; row++) {
		t_row *prev_row = board->rows[row - 1];
		t_row *cur_row = board->rows[row];
		Player prev_finisher = prev_row->pref_finisher;

		if (prev_finisher == AI) {
			cur_row->pref_finisher =
			    (prev_row->start_amount % 4 == 0) ? AI : PLAYER;
		}
		else {
			cur_row->pref_finisher =
			    (prev_row->start_amount % 4 == 1) ? AI : PLAYER;
		}
	}
}

#include "_init.h"
#include "alcu.h"
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
static Result allocate_picks(t_board *board);

// If filename == NULL, read from stdin
Result init_board(t_board *board, const char *filename)
{
	Result res = read_board(board, filename);

	if (res == RESULT_OK) {
		board->width = max_width(board);
		board->cur_row = board->height - 1;
		res = allocate_picks(board);
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
	free(board->picks);
	free_get_next_line();
}

static Result read_board(t_board *board, const char *filename)
{
	t_list *rows = NULL;
	Result res = read_rows(&rows, filename);

	if (res == RESULT_OK) {
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

static Result allocate_picks(t_board *board)
{
	size_t matches = 0;

	for (size_t row = 0; row < board->height; row++) {
		matches += board->rows[row]->start_amount;
	}
	board->picks = ft_calloc(matches + 1, sizeof(unsigned int));
	if (board->picks == NULL) {
		return INTERNAL_ERROR;
	}
	return RESULT_OK;
}

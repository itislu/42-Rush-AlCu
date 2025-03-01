#include "board.h"

static bool read_board(t_board *board, int fd);

// If filename == NULL, read from stdin
Result init_board(t_board *board, const char *filename)
{
	(void)board;
	(void)filename;
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

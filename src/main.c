#include "board.h"
#include "ft_printf.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
	t_board board;

	if (argc > 2) {
		ft_dprintf(STDERR_FILENO, "%s [file]\n", argv[0]);
		return USAGE_ERROR;
	}

	Result res = init_board(&board, argv[1]);
	if (res != OK) {
		return res;
	}

	free_board(&board);
	return OK;
}

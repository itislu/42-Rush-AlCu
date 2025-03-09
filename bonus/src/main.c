#include "alcu.h"
#include "ft_printf.h"
#include "init.h"
#include "libft.h"
#include "print.h"
#include "visuals.h"
#include <ncurses.h>
#include <stddef.h>
#include <sys/param.h>
#include <unistd.h>

int g_stdin = STDIN_FILENO;

int main(int argc, char *argv[])
{
	Result res = RESULT_OK;
	t_board board;
	t_ncurses env;
	ft_bzero(&board, sizeof(board));
	ft_bzero(&env, sizeof(env));

	if (argc > 2) {
		res = USAGE_ERROR;
	}
	if (res == RESULT_OK) {
		res = init(&board, &env, argv[1]);
	}
	if (res == RESULT_OK) {
		res = game_loop(&board, &env);
	}
	cleanup_ncruses(&env);

	switch (res) {
	case RESULT_OK:
		print_result(&board);
		break;
	case USAGE_ERROR:
		ft_dprintf(STDERR_FILENO, "%s <file>\n", argv[0]);
		break;
	case BOARD_ERROR:
		ft_dprintf(STDERR_FILENO, "ERROR\n");
		break;
	case INTERNAL_ERROR:
		ft_dprintf(STDERR_FILENO, "INTERNAL ERROR\n");
		break;
	case USER_EXIT:
		clear_line();
		const char title[] = "Bye-Bye! 👋";
		print_boxed_specialstr(title, sizeof(title) - 1 - 2);
		res = RESULT_OK;
		break;
	case SIZE_ERROR:
		ft_dprintf(STDERR_FILENO,
		           "MIN TERMINAL SIZE: %ux%u\n",
		           MIN_TERMINAL_WIDTH,
		           MIN_TERMINAL_HEIGTH);
		break;
	}

	free_board(&board);
	return res;
}

#include "alcu.h"
#include "ft_printf.h"
#include "libft.h"
#include "print_utils.h"
#include <stddef.h>
#include <unistd.h>

static Result game_loop(t_board *board);
static bool is_game_end(t_board *board);
static void print_result(t_board *board);

int main(int argc, char *argv[])
{
	Result res = OK;
	t_board board;

	if (argc > 2) {
		res = USAGE_ERROR;
	}
	if (res == OK) {
		ft_printf("\033[1m" TITLE "\033[0m");
		res = init_board(&board, argv[1]);
	}
	if (res == OK) {
		print_board_complete(&board);
		ft_printf("\n");
		res = prompt_game_mode(&board.game_mode);
	}
	if (res == OK) {
		calc_finishers(&board);
		res = game_loop(&board);
	}

	switch (res) {
	case OK:
		print_result(&board);
		break;
	case USER_EXIT:
		clear_line();
		const char title[] = "Bye-Bye! 👋";
		print_boxed_specialstr(title, sizeof(title) - 1 - 2);
		break;
	case INTERNAL_ERROR:
		ft_dprintf(STDERR_FILENO, "INTERNAL ERROR\n");
		break;
	case USAGE_ERROR:
		ft_dprintf(STDERR_FILENO, "%s [file]\n", argv[0]);
		break;
	case BOARD_ERROR:
		ft_dprintf(STDERR_FILENO, "ERROR\n");
		break;
	}

	free_board(&board);
	return res;
}

static Result game_loop(t_board *board)
{
	Result res = OK;
	Player cur_player = AI;

	while (!is_game_end(board)) {
		t_row *cur_row = board->rows[board->cur_row];
		unsigned int picks = 0;

		if (cur_player == AI) {
			picks = ai_pick(board);
		}
		else {
			res = prompt_picks(board, &picks);
			if (res != OK) {
				break;
			}
		}
		cur_row->cur_amount -= picks;
		cur_row->last_pick = cur_player;

		print_board_gameloop(board, picks);

		if (cur_row->cur_amount == 0 && board->cur_row != 0) {
			board->cur_row--;
		}
		cur_player *= -1;
	}
	return res;
}

static bool is_game_end(t_board *board)
{
	return board->cur_row == 0 && board->rows[board->cur_row]->cur_amount == 0;
}

static void print_result(t_board *board)
{
	const char *title = NULL;

	if ((board->game_mode == LAST_WINS && board->rows[0]->last_pick == PLAYER)
	    || (board->game_mode == LAST_LOSES
	        && board->rows[0]->last_pick == AI)) {
		title = "🌟 Congrats, you won! 🌟";
	}
	else {
		title = "🥀 You lost... 🥀";
	}
	print_boxed_specialstr(title, ft_strlen(title) - 4);
}

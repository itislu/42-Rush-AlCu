#include "board.h"
#include "ft_printf.h"
#include "libft.h"
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
		res = init_board(&board, argv[1]);
	}
	if (res == OK) {
		res = game_loop(&board);
	}

	switch (res) {
	case OK:
		print_result(&board);
		break;
	case USER_EXIT:
		ft_printf("\nBye-Bye! 👋\n");
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

	// print_board();
	while (!is_game_end(board)) {
		t_row *cur_row = board->rows[board->cur_row];
		unsigned int picks = 0;

		if (cur_player == AI) {
			if (cur_row->pref_finisher == AI) {
				picks = ft_max(1, cur_row->cur_amount % 4);
			}
			else {
				picks = ft_max(1, (cur_row->cur_amount - 1) % 4);
			}
		}
		else {
			res = prompt_picks(board, &picks);
			if (res != OK) {
				break;
			}
		}
		cur_row->cur_amount -= picks;
		cur_row->last_pick = cur_player;
		if (cur_row->cur_amount == 0 && board->cur_row != 0) {
			board->cur_row--;
		}

		// print_board();

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
	if ((board->game_mode == LAST_WINS && board->rows[0]->last_pick == PLAYER)
	    || (board->game_mode == LAST_LOSES
	        && board->rows[0]->last_pick == AI)) {
		ft_printf("🌟 Congrats, you won! 🌟\n");
	}
	else {
		ft_printf("🥀 You lost... 🥀\n");
	}
}

#include "board.h"
#include "ft_printf.h"
#include "libft.h"
#include <unistd.h>

static bool is_game_end(t_board *board);
static void print_result(t_board *board);

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

	Player cur_player = AI;
	// print_board();
	while (!is_game_end(&board)) {
		t_row *cur_row = &board.rows[board.cur_row];
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
			picks = prompt(&board);
			if (picks == 0){
				return (0); //EOF
			}
		}
		cur_row->cur_amount -= picks;
		cur_row->last_pick = cur_player;
		if (cur_row->cur_amount == 0 && board.cur_row != 0) {
			board.cur_row--;
		}

		// print_board();

		cur_player *= -1;
	}

	print_result(&board);

	free_board(&board);
	return OK;
}

static bool is_game_end(t_board *board)
{
	return board->cur_row == 0 && board->rows[board->cur_row].cur_amount == 0;
}

static void print_result(t_board *board)
{
	if (board->rows[0].last_pick == board->game_mode ) {
		ft_printf("Congrats! Whatev...\n");
	}
	else {
		ft_printf("Sucker\n");
	}
}

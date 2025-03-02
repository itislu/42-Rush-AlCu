
#include "alcu.h"
#include "libft.h"
#include <stddef.h>

void calc_finishers(t_board *board)
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

unsigned int ai_pick(t_board *board)
{
	t_row *cur_row = board->rows[board->cur_row];
	unsigned int picks = 0;

	if (cur_row->pref_finisher == AI) {
		picks = ft_max(1, cur_row->cur_amount % 4);
	}
	else {
		picks = ft_max(1, (cur_row->cur_amount - 1) % 4);
	}
	return picks;
}

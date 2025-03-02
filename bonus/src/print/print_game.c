#include "alcu.h"
#include "libft.h"
#include "print.h"
#include <stddef.h>

void print_game_mode(Mode mode)
{
	const char *title = NULL;

	if (mode == LAST_LOSES) {
		title = "🚨 LAST PICK LOSES 🚨";
	}
	else {
		title = "🏅 LAST PICK WINS 🏅";
	}
	print_boxed_specialstr(title, ft_strlen(title) - 4);
}

void print_result(t_board *board)
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

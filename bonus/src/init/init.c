#include "init.h"
#include "ai.h"
#include "alcu.h"
#include "print.h"
#include "visuals.h"

Result init(t_board *board, t_ncurses *env, char *filename)
{
	Result res = RESULT_OK;

	const char title[] = "ℹ️  Maximize the terminal for the best experience ℹ️ ";
	print_boxed_specialstr(title, sizeof(title) - 1 - 10);

	res = init_board(board, filename);

	if (res == RESULT_OK) {
		res = setup_ncurses(env);
	}
	if (res == RESULT_OK) {
		update_board(&env->board, board);
		update_history(&env->history, board);
		res = game_mode_selection(env, board);
	}
	if (res == RESULT_OK) {
		calc_finishers(board);
	}
	return res;
}

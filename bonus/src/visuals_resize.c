#include "visuals.h"

Result	resize_window(int ch, t_ncurses *env, t_board *board)
{
	Result	res;

	if (ch != KEY_RESIZE)
		return (RESULT_OK);
	// destroy old environment
	erase(); // clear the standard screen => removes borders lefthand side
	cleanup_ncruses(env);
	// create new environment
	res = setup_ncurses(env);
	if (res != RESULT_OK)
		return (res);
	// fill with content
	(void) board;
	// redraw new windows
	refresh(); // refreshes standard screen => removes borders righthand side
	update_board(&env->board, board);
	update_history(&env->history, board);
	update_input(&env->input, board);
	return (RESULT_OK);
}

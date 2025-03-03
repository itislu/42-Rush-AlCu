#include "visuals.h"

Result	resize_window(int ch, t_ncurses *env, t_board *board)
{
	Result	res;

	if (ch != KEY_RESIZE)
		return (RESULT_OK);
	// destroy old environment
	cleanup_ncruses(env);
	// creaaate new environment
	res = setup_ncurses(env);
	if (res != RESULT_OK)
		return (res);
	// fill with content
	(void) board;
	// redraw new windows
	wrefresh(env->board.win);
	wrefresh(env->history.win);
	wrefresh(env->input.win);
	return (RESULT_OK);
}

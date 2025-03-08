#include "visuals.h"

Result	resize_window(int ch, t_ncurses *env, t_board *board)
{
	Result	res;

	if (ch != KEY_RESIZE)
		return (RESULT_OK);
	// destroy old environment
	erase(); // clear the standard screen => removes content left&top (increase)
	cleanup_ncruses(env);
	// create new environment
	res = setup_ncurses(env);
	if (res != RESULT_OK)
		return (res);
	refresh(); // refresh standard screen => remove content right&down (shrink)
	// adjust scroll offset to show more lines when increasing window size
	if (env->board.scroll_offset
		&& env->board.size.y - 3 > board->cur_row - env->board.scroll_offset) {
			env->board.scroll_offset--;
		}
	if (env->history.scroll_offset
		&& env->history.size.y - 3 > board->cur_turn - env->history.scroll_offset) {
			env->history.scroll_offset--;
		}
	// redraw new windows
	update_board(&env->board, board);
	update_history(&env->history, board);
	update_input(&env->input, board);
	return (RESULT_OK);
}

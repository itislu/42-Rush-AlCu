#include "visuals.h"
// here to shutup the blue squiggles...
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

static void	test_draw(t_ncurses *env)
{
	// sample text inside all windows
	mvwprintw(env->board.win, 1, 1, "Board");
	mvwprintw(env->history.win, 1, 1, "History");
	mvwprintw(env->input.win, 1, 1, "Input");
	// draw the windows
	wrefresh(env->board.win);
	wrefresh(env->history.win);
	wrefresh(env->input.win);
	
}

// void	handle_input_window(t_ncurses *env, int key)
// {
// 	// get the next move thing
// }

// void	handle_history_window(t_ncurses *env, int key)
// {
// 	// update print from the board
// }

void	handle_board_window(t_ncurses *env, t_board *board)
{
	int	xoffset;

	for (size_t i = 0; i < board->height; i++) {
		xoffset = 1 + i;
		if (xoffset != 0 && xoffset != (getmaxy(env->board.win) - 1))
			mvwprintw(env->board.win, xoffset, 2, " line %zu", i);
		wrefresh(env->board.win);
	}
	(void) env; (void) board;
}

// void	handle_mouse_event(t_ncurses *env, int key, MEVENT *mouse)
// {
// 	// scrolling for each window
// }

// void	run_visuals(t_board *board)
Result	run_visuals(t_board *board)
{
	t_ncurses	env;
	// MEVENT		mouse;
	int key = 0;

	setup_ncurses(&env);
	// inital draw windows?
	test_draw(&env);
	while (key != 'q' && key != 27)
	{
		// input window
		// handle_input_window(&env, key);
		// history window
		// handle_history_window(&env, key);
		// board window
		handle_board_window(&env, board);
		// if (key == KEY_MOUSE) {
		// 	getmouse(&mouse);
		// 	handle_mouse_event(&env, key, &mouse);
		// }
		key = wgetch(env.input.win);
	}
	cleanup_ncruses(&env);
	return (RES_OK);
}

// int main(void)
// {
// 	run_visuals();
// 	return (0);
// }

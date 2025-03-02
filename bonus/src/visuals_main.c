#include "../inc/visuals.h"
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

// void	run_visuals(t_board *board)
void	run_visuals(void)
{
	t_ncurses	env;
	int ch = 0;

	setup_ncurses(&env);
	// inital draw windows?
	test_draw(&env);
	// some loop?
	while ((ch = wgetch(env.input.win)) != 'q')
	{
		// ch = getch();
		// ;
		// wrefresh(env->board.win);
		// wrefresh(env->history.win);
		// wrefresh(env->input.win);
	}
	cleanup_ncruses(&env);
}

// int main(void)
// {
// 	run_visuals();
// 	return (0);
// }

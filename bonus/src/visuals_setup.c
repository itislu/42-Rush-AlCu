#include "visuals.h"
// here to shutup the blue squiggles...
#include <ncurses.h>
#include <stdlib.h>

static void	init_ncurses(void)
{
	setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    curs_set(0);	// hide cursor
    // keypad(stdscr, TRUE); // we use windows, don't need that, this is for the terminal
    mousemask(ALL_MOUSE_EVENTS, NULL);
	ESCDELAY = 25;
}

static void	setup_windows_sizes(t_ncurses *env)
{
	env->board.size.x = env->term.x * 2 / 3 - 1; // -1 for the border not to overlap
	env->board.size.y = env->term.y - 1; // this might be the missing 1 for the +3 offset
	env->history.size.x = env->term.x / 3;
	env->history.size.y = env->term.y * 2 / 3 - 1;
	env->input.size.x = env->term.x / 3;
	env->input.size.y = env->term.y / 3 + (env->term.y / 3) % 2; // fix for height
}

static void	setup_windows_position(t_ncurses *env)
{
	env->board.pos.x = 1;
	env->board.pos.y = 1;
	env->history.pos.x = env->term.x * 2 / 3;
	env->history.pos.y = 1;
	env->input.pos.x = env->term.x * 2 / 3;
	env->input.pos.y = env->term.y * 2 / 3;
}

static bool	init_windows_err(t_ncurses *env)
{
	getmaxyx(stdscr, env->term.y, env->term.x);
	setup_windows_sizes(env);
	setup_windows_position(env);
	env->board.win = newwin(env->board.size.y, env->board.size.x, env->board.pos.y, env->board.pos.x);
	if (!env->board.win)
		return (true);
	env->history.win = newwin(env->history.size.y, env->history.size.x, env->history.pos.y, env->history.pos.x);
	if (!env->history.win)
		return (true);
	env->input.win = newwin(env->input.size.y, env->input.size.x, env->input.pos.y, env->input.pos.x);
	if (!env->input.win)
		return (true);
	// draw the borders for the windows, uses TOP and BOTTOM line for the border
	box(env->board.win, 0, 0);
	box(env->history.win, 0, 0);
	box(env->input.win, 0, 0);
	// activate keyboard input for the input window
	keypad(env->input.win, TRUE);
	return (false);
}
void	cleanup_ncruses(t_ncurses *env)
{
	if (env->board.win)
		delwin(env->board.win);
	if (env->history.win)
		delwin(env->history.win);
	if (env->input.win)
		delwin(env->input.win);
	endwin();
}

void	setup_ncurses(t_ncurses *env)
{
	init_ncurses();
	if (init_windows_err(env)) { // needs proper game exit
		cleanup_ncruses(env);
		//exit(1);
	}
}

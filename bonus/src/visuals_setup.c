#include "alcu.h"
#include "visuals.h"
#include <ncurses.h>

static Result init_windows(t_ncurses *env);

static void init_ncurses(void)
{
	initscr();
	// init needs a success check - no return values here are checked
	noecho(); // user typed input will not be displayed on screen
	cbreak(); // disables line buffering; typed characters immediately available
	curs_set(0); // hide CLI cursor
	mousemask(ALL_MOUSE_EVENTS, NULL);
	ESCDELAY = 25;
}

static void setup_windows_sizes(t_ncurses *env)
{
	unsigned int info_width;
	if (env->term.x / 3 > MAX_INFO_PANEL_WIDTH)
		info_width = MAX_INFO_PANEL_WIDTH;
	else
		info_width = MIN_INFO_PANEL_WIDTH;
	// env->board.size.x = env->term.x * 2 / 3;
	env->board.size.x = env->term.x - info_width;
	env->board.size.y = env->term.y;
	env->input.size.x = info_width;
	env->history.size.x = info_width;
	if (env->term.y > MIN_TERMINAL_HEIGTH_FOR_HISTORY) {
		env->input.size.y = MIN_TERMINAL_HEIGTH;
		env->history.size.y = env->term.y - env->input.size.y;
	}
	else {
		env->input.size.y = env->board.size.y;
		env->history.size.y = 0;
	}
}

static void setup_windows_position(t_ncurses *env)
{
	env->board.pos.x = 0;
	env->board.pos.y = 0;
	env->history.pos.y = 0;
	env->is_history = true;
	env->history.pos.x = env->board.size.x;
	env->input.pos.x = env->board.size.x;
	if (env->term.y > MIN_TERMINAL_HEIGTH_FOR_HISTORY) {
		env->input.pos.y = env->history.size.y;
	}
	else {
		env->is_history = false;
		env->input.pos.y = 0;
	}
}
static Result terminal_size(t_ncurses *env)
{
	keypad(stdscr, TRUE);
	mvprintw(1,1,"terminal too small");
	mvprintw(2,1,"size (%i,%i) need (%i,%i)",
		env->term.x, env->term.y, MIN_TERMINAL_WIDTH, MIN_TERMINAL_HEIGTH);
	refresh();
	int ch = getch();
	if (ch == ESCAPE || ch == 'q')
		return (INTERNAL_ERROR); // this needs proper exit cleanup code
	else if (ch != KEY_RESIZE)
		return (RESULT_OK);
	getmaxyx(stdscr, env->term.y, env->term.x);
	keypad(stdscr, FALSE);
	return (RESULT_OK);
}

static Result init_windows(t_ncurses *env)
{
	getmaxyx(stdscr, env->term.y, env->term.x);
	while (env->term.y < MIN_TERMINAL_HEIGTH || env->term.x < MIN_TERMINAL_WIDTH) {
		if (terminal_size(env) != RESULT_OK)
			return (INTERNAL_ERROR);
	}
	setup_windows_sizes(env);
	setup_windows_position(env);
	env->board.win = newwin(env->board.size.y,
	                        env->board.size.x,
	                        env->board.pos.y,
	                        env->board.pos.x);
	if (!env->board.win) {
		return (INTERNAL_ERROR);
	}
	env->history.win = newwin(env->history.size.y,
	                          env->history.size.x,
	                          env->history.pos.y,
	                          env->history.pos.x);
	if (!env->history.win) {
		return (INTERNAL_ERROR);
	}
	env->input.win = newwin(env->input.size.y,
	                        env->input.size.x,
	                        env->input.pos.y,
	                        env->input.pos.x);
	if (!env->input.win) {
		return (INTERNAL_ERROR);
	}
	// draw the borders for the windows, uses TOP and BOTTOM line for the border
	box(env->board.win, 0, 0);
	box(env->history.win, 0, 0);
	box(env->input.win, 0, 0);
	// activate keyboard input for the input window
	keypad(env->input.win, TRUE);
	return (RESULT_OK);
}
void cleanup_ncruses(t_ncurses *env)
{
	if (env->board.win) {
		delwin(env->board.win);
	}
	if (env->history.win) {
		delwin(env->history.win);
	}
	if (env->input.win) {
		delwin(env->input.win);
	}
	endwin();
}

Result setup_ncurses(t_ncurses *env)
{
	Result res = RESULT_OK;

	init_ncurses(); // this probably needs error check
	res = init_windows(env);
	if (res != RESULT_OK) {
		cleanup_ncruses(env);
	}
	return (res);
}

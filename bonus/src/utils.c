#include "alcu.h"
#include "visuals.h"
#include <ncurses.h>
#include <stddef.h>
#include <sys/param.h>
#include <unistd.h>
#include "libft.h"

size_t capped_sub(size_t a, size_t b)
{
	return a > b ? a - b : 0;
}

WINDOW *detect_window(t_ncurses *env, unsigned int y, unsigned int x)
{
	if (env->is_history
		&& y >= env->history.pos.y
	    && y < env->history.pos.y + env->history.size.y - 1
	    && x >= env->history.pos.x
	    && x < env->history.pos.x + env->history.size.x - 1) {
		return env->history.win;
	}
	if (y >= env->input.pos.y && y < env->input.pos.y + env->input.size.y - 1
	    && x >= env->input.pos.x
	    && x < env->input.pos.x + env->input.size.x - 1) {
		return env->input.win;
	}
	return env->board.win;
}

void print_title(t_win *win, char *title)
{
	werase(win->win);
	box(win->win, 0, 0);
	wattron(win->win, A_BOLD);
	mvwprintw(win->win, 1, (win->size.x - ft_strlen(title) + 1) / 2, title);
	wattroff(win->win, A_BOLD);
	wrefresh(win->win);
}

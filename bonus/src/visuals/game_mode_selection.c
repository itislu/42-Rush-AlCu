#include "alcu.h"
#include "visuals.h"
#include <ncurses.h>

Result game_mode_selection(t_ncurses *env, t_board *board)
{
	Result res = RESULT_OK;
	board->game_mode = 0;
	board->num_options = 2;

	const char *options[2] = {"Last to pick loses", "Last to pick wins"};

	env->input.scroll_offset = 0;
	int ch = 0;
	while (ch != '\n') {
		werase(env->input.win);
		box(env->input.win, 0, 0);
		mvwprintw(env->input.win, 1, 1, "Select game mode:");

		for (unsigned int i = 0; i < 2; i++) {
			if (i == env->input.scroll_offset) {
				wattron(env->input.win, A_REVERSE);
			}
			mvwprintw(env->input.win, 3 + i, 2, "%s", options[i]);
			wattroff(env->input.win, A_REVERSE);
		}
		wrefresh(env->input.win);
		ch = wgetch(env->input.win);
		if (ch == 'q' || ch == ESCAPE) {
			res = USER_EXIT;
			return res;
		}
		scroll_handler(board, env, ch);
	}
	board->game_mode = env->input.scroll_offset == 0 ? LAST_LOSES : LAST_WINS;
	return res;
}

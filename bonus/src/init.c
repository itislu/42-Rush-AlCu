#include "ai.h"
#include "alcu.h"
#include "init.h"
#include "print.h"
#include "visuals.h"
#include <ncurses.h>
#include <stddef.h>
#include <sys/param.h>
#include <unistd.h>

Result init_bonus(t_board *board, t_ncurses *env, char *filename)
{
	Result res = RESULT_OK;

	const char title[] =
		"ℹ️  Maximize the terminal for the best experience ℹ️ ";
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
		if (scroll_handler(board, env, ch)) {
			break;
		}
	}
	board->game_mode = env->input.scroll_offset == 0 ? LAST_LOSES : LAST_WINS;
	return res;
}

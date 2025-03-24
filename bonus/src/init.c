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
		print_title(&env->history, HISTORY_TITLE);
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

	const char *options[2] = {LAST_TO_PICK_LOSES, LAST_TO_PICK_WINS};

	env->input.scroll_offset = 0;
	int ch = 0;
	while (ch != '\n') {
		print_title(&env->input, SELECT_GAME_MODE);

		for (unsigned int i = 0; i < 2; i++) {
			if (i == env->input.scroll_offset) {
				wattron(env->input.win, A_REVERSE);
			}
			mvwprintw(env->input.win, env->input.title_height + 1 + i, 2, "%s", options[i]);
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
		res = resize_window(ch, env, board);
		if (res != RESULT_OK)
			return (res);
	}
	board->game_mode = env->input.scroll_offset == 0 ? LAST_LOSES : LAST_WINS;
	return res;
}

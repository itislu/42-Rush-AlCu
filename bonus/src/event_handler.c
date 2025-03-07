#include "alcu.h"
#include "visuals.h"
#include <ncurses.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/param.h>

void mouse(t_ncurses *env, t_board *board)
{
	MEVENT event;
	if (getmouse(&event) == OK) {
		WINDOW *win = detect_window(env, event.y, event.x);
		if (event.bstate & BUTTON5_PRESSED)
		{
			if (win == env->board.win && env->board.scroll_offset > 1) {
				env->board.scroll_offset--;
				update_board(&env->board, board);
			}
			else if (win == env->history.win && env->is_history
			         && env->history.scroll_offset > 1) {
				env->history.scroll_offset--;
				update_history(&env->history, board);
			}
			else if (win == env->input.win && !is_game_end(board)) {
				env->input.scroll_offset =
				    (env->input.scroll_offset + 1 + board->num_options)
				    % board->num_options;
				update_input(&env->input, board);
			}
		}
		else if (event.bstate & BUTTON4_PRESSED)
		{
			if (win == env->board.win && board->cur_row > env->board.size.y - 4
			    && env->board.scroll_offset
			           < board->cur_row - env->board.size.y + 4) {
				env->board.scroll_offset++;
				update_board(&env->board, board);
			}
			else if (win == env->history.win && env->is_history
			         && board->cur_turn > env->history.size.y - 4
			         && env->history.scroll_offset
			                < board->cur_turn - env->history.size.y + 4)
			{
				env->history.scroll_offset++;
				update_history(&env->history, board);
			}
			else if (win == env->input.win && !is_game_end(board)) {
				env->input.scroll_offset =
				    (env->input.scroll_offset - 1 + board->num_options)
				    % board->num_options;
				update_input(&env->input, board);
			}
		}
	}
}

void scroll_handler(t_board *board, t_ncurses *env, int ch)
{
	if (ch == KEY_UP && !is_game_end(board)) {
		env->input.scroll_offset =
		    (env->input.scroll_offset - 1 + board->num_options)
		    % board->num_options;
		update_input(&env->input, board);
	}
	else if (ch == KEY_DOWN && !is_game_end(board)) {
		env->input.scroll_offset =
		    (env->input.scroll_offset + 1) % board->num_options;
		update_input(&env->input, board);
	}
	else if (ch == KEY_MOUSE) {
		mouse(env, board);
	}
}

#include "ai.h"
#include "alcu.h"
#include "visuals.h"
#include <ncurses.h>
#include <stddef.h>
#include <sys/param.h>
#include <unistd.h>

bool is_game_end(t_board *board)
{
	return board->cur_row == 0 && board->rows[board->cur_row]->cur_amount == 0;
}

void turn(t_board *board, t_ncurses *env, Player *player)
{
	if (is_game_end(board)) {
		return;
	}

	t_row *cur_row = board->rows[board->cur_row];
	unsigned int picks = 0;

	if (*player == PLAYER) {
		picks = env->input.scroll_offset + 1;
	}
	else {
		picks = ai_pick(board);
	}
	cur_row->cur_amount -= picks;
	cur_row->last_pick = *player;
	board->picks[board->cur_turn] = picks;

	if (env->is_history) {
		env->history.scroll_offset = 0;
		update_history(&env->history, board);
	}
	if (cur_row->cur_amount == 0 && board->cur_row != 0) {
		board->cur_row--;
	}
	board->cur_turn++;
	*player *= -1;
	env->board.scroll_offset = 0;
	update_board(&env->board, board);
}

Result game_loop(t_board *board, t_ncurses *env)
{
	Result res = RESULT_OK;
	Player player = AI;

	env->input.scroll_offset = 0;
	turn(board, env, &player);
	board->num_options = MIN(board->rows[board->cur_row]->cur_amount, 3);
	update_input(&env->input, board);
	while (true) {
		int ch = wgetch(env->input.win);

		if ((ch == '\n' || ch == KEY_RIGHT) && !is_game_end(board)) {
		pick:
			turn(board, env, &player);
			turn(board, env, &player);
			board->num_options =
			    MIN(board->rows[board->cur_row]->cur_amount, 3);
			env->input.scroll_offset = 0;
			update_input(&env->input, board);
		}
		else if (ch == 'q' || ch == ESCAPE) {
			if (!is_game_end(board)) {
				res = USER_EXIT;
			}
			break;
		}
		else if (ch == '1' && !is_game_end(board) && board->num_options >= 1) {
			env->input.scroll_offset = 0;
			goto pick;
		}
		else if (ch == '2' && !is_game_end(board) && board->num_options >= 2) {
			env->input.scroll_offset = 1;
			goto pick;
		}
		else if (ch == '3' && !is_game_end(board) && board->num_options == 3) {
			env->input.scroll_offset = 2;
			goto pick;
		}
		else {
			scroll_handler(board, env, ch);
		}
	}
	return res;
}

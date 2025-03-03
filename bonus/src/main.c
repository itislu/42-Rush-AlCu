#include "ai.h"
#include "alcu.h"
#include "ft_printf.h"
#include "init.h"
#include "libft.h"
#include "print.h"
#include "visuals.h"
#include <ncurses.h>
#include <stddef.h>
#include <sys/param.h>
#include <unistd.h>

static Result game_loop(t_board *board, t_ncurses *env);
static bool is_game_end(t_board *board);
void update_history(t_win *history, t_board *board);
Result game_mode_selection(t_ncurses *env, t_board *board);
void scroll_handler(t_board *board, t_ncurses *env, int ch);

int g_stdin = STDIN_FILENO;

int main(int argc, char *argv[])
{
	Result res = RESULT_OK;
	t_board board;
	t_ncurses env;
	ft_bzero(&board, sizeof(board));
	ft_bzero(&env, sizeof(env));

	if (argc > 2) {
		res = USAGE_ERROR;
	}
	if (res == RESULT_OK) {
		const char title[] =
		    "ℹ️  Maximize the terminal for the best experience ℹ️ ";
		print_boxed_specialstr(title, sizeof(title) - 1 - 10);
		res = init_board(&board, argv[1]);
	}
	if (res == RESULT_OK) {
		// ft_printf("\033[1m" TITLE "\033[0m");
		res = setup_ncurses(&env);
	}
	if (res == RESULT_OK) {
		// print_board_complete(&board);
		// ft_printf("\n");
		// res = prompt_game_mode(&board.game_mode); // TODO Popup
		update_board(&env.board, &board);
		update_history(&env.history, &board);
		res = game_mode_selection(&env, &board);
	}
	if (res == RESULT_OK) {
		calc_finishers(&board);
		res = game_loop(&board, &env);
	}
	cleanup_ncruses(&env);

	switch (res) {
	case RESULT_OK:
		print_result(&board);
		break;
	case USAGE_ERROR:
		ft_dprintf(STDERR_FILENO, "%s <file>\n", argv[0]);
		break;
	case BOARD_ERROR:
		ft_dprintf(STDERR_FILENO, "ERROR\n");
		break;
	case INTERNAL_ERROR:
		ft_dprintf(STDERR_FILENO, "INTERNAL ERROR\n");
		break;
	case USER_EXIT:
		clear_line();
		const char title[] = "Bye-Bye! 👋";
		print_boxed_specialstr(title, sizeof(title) - 1 - 2);
		res = RESULT_OK;
		break;
	case SIZE_ERROR:
		ft_dprintf(STDERR_FILENO,
		           "MIN TERMINAL SIZE: %dx%d\n",
		           MIN_TERMINAL_WIDTH,
		           MIN_TERMINAL_HEIGTH);
		break;
	}

	free_board(&board);
	return res;
}

size_t capped_sub(size_t a, size_t b)
{
	return a > b ? a - b : 0;
}

static void turn(t_board *board, t_ncurses *env, Player *player)
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

void update_history(t_win *history, t_board *board)
{
	int y_offset = 2;
	unsigned int start = 0;

	werase(history->win);
	box(history->win, 0, 0);
	mvwprintw(history->win, 1, 1, "History:");
	// offset *= -1;
	// offset += capped_sub(board->cur_turn, history->size.y - 4);
	// start = capped_sub(board->cur_turn, history->size.y - 4) - offset;
	if (board->cur_turn - history->scroll_offset > history->size.y - 4) {
		start = board->cur_turn - history->scroll_offset - history->size.y + 4;
	}
	for (unsigned int i = start; i <= board->cur_turn - history->scroll_offset;
	     i++) {
		// for (unsigned int i = offset; i <= board->cur_turn; i++) {
		mvwprintw(history->win,
		          y_offset++,
		          1,
		          "#%i %s picked %d",
		          i + 1,
		          i % 2 == 0 ? "AI" : "You",
		          board->picks[i]); // offset does NOT work with scrolling
		                            // either fix the capped_sub below or remove
		                            // scrolling
		// capped_sub(board->cur_turn, history->size.y  - i)]);
	}
	wrefresh(history->win);
}

// void update_board(t_win *board_win, t_board *board) {
// 	werase(board_win->win);
// 	box(board_win->win, 0, 0);
// 	//something
// 	(void)board;
// 	wrefresh(board_win->win);
// }

void print_res(t_win input, t_board *board)
{
	Player winner;
	if ((board->game_mode == LAST_WINS && board->rows[0]->last_pick == PLAYER)
	    || (board->game_mode == LAST_LOSES
	        && board->rows[0]->last_pick == AI)) {
		winner = PLAYER;
	}
	else {
		winner = AI;
	}

	werase(input.win);
	box(input.win, 0, 0);
	// if mode is LAST_LOSES, the last player to pick wins
	mvwprintw(input.win,
	          1,
	          1,
	          "%s",
	          winner == PLAYER ? "Congrats, you won!" : "You lost...");
	wrefresh(input.win);
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
		else {
			scroll_handler(board, env, ch);
		}
	}
	board->game_mode = env->input.scroll_offset == 0 ? LAST_LOSES : LAST_WINS;
	return res;
}

void update_input(t_win input, t_board *board)
{
	if (board->game_mode == 0) {
		return;
	}
	if (is_game_end(board)) {
		print_res(input, board);
		return;
	}

	const char *options[3] = {"1 pick", "2 picks", "3 picks"};

	werase(input.win);
	box(input.win, 0, 0);
	mvwprintw(input.win, 1, 1, "Select how many picks you want to remove:");

	for (unsigned int i = 0; i < board->num_options; i++) {
		if (i == input.scroll_offset) {
			wattron(input.win, A_REVERSE);
		}
		mvwprintw(input.win, 3 + i, 2, "%s", options[i]);
		wattroff(input.win, A_REVERSE);
	}
	wrefresh(input.win);
}

WINDOW *detect_window(t_ncurses *env, unsigned int y, unsigned int x)
{
	if (y >= env->history.pos.y
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

void mouse(t_ncurses *env, t_board *board)
{
	MEVENT event;
	if (getmouse(&event) == OK) {
		// WINDOW *win = env->board.win;
		WINDOW *win = detect_window(env, event.y, event.x);
		if (event.bstate & BUTTON5_PRESSED) // scrolldown
		{
			if (win == env->board.win && env->board.scroll_offset > 0) {
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
				update_input(env->input, board);
			}
		}
		else if (event.bstate & BUTTON4_PRESSED) // scrollup
		{
			if (win == env->board.win && board->cur_row > env->board.size.y - 3
			    && env->board.scroll_offset
			           < board->cur_row - env->board.size.y + 3) {
				env->board.scroll_offset++;
				update_board(&env->board, board);
			}
			else if (win == env->history.win && env->is_history
			         && board->cur_turn > env->history.size.y - 4
			         && env->history.scroll_offset
			                < board->cur_turn - env->history.size.y + 4)
			// env->history.size.y - 4 // is the limit to print the last row
			{
				env->history.scroll_offset++;
				update_history(&env->history, board);
			}
			else if (win == env->input.win && !is_game_end(board)) {
				env->input.scroll_offset =
				    (env->input.scroll_offset - 1 + board->num_options)
				    % board->num_options;
				update_input(env->input, board);
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
		update_input(env->input, board);
	}
	else if (ch == KEY_DOWN && !is_game_end(board)) {
		env->input.scroll_offset =
		    (env->input.scroll_offset + 1) % board->num_options;
		update_input(env->input, board);
	}
	else if (ch == KEY_MOUSE) {
		mouse(env, board);
	}
}

static Result game_loop(t_board *board, t_ncurses *env)
{
	Result res = RESULT_OK;
	Player player = AI;

	env->input.scroll_offset = 0;
	turn(board, env, &player);
	board->num_options = MIN(board->rows[board->cur_row]->cur_amount, 3);
	update_input(env->input, board);
	while (true) {
		int ch = wgetch(env->input.win);

		if ((ch == '\n' || ch == KEY_RIGHT) && !is_game_end(board)) {
		pick:
			turn(board, env, &player);
			turn(board, env, &player);
			board->num_options =
			    MIN(board->rows[board->cur_row]->cur_amount, 3);
			env->input.scroll_offset = 0;
			update_input(env->input, board);
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

static bool is_game_end(t_board *board)
{
	return board->cur_row == 0 && board->rows[board->cur_row]->cur_amount == 0;
}

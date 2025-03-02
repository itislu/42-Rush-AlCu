#include "ai.h"
#include "alcu.h"
#include "ft_printf.h"
#include "init.h"
#include "libft.h"
#include "print.h"
#include "prompt.h"
#include <stddef.h>
#include <sys/param.h>
#include <unistd.h>
#include "visuals.h"
#include <ncurses.h>

static Result game_loop(t_board *board, t_ncurses *env);
static bool is_game_end(t_board *board);
void update_history(t_win history, t_board *board, int offset);

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
		res = init_board(&board, argv[1]);
	}
	if (res == RESULT_OK) {
		//ft_printf("\033[1m" TITLE "\033[0m");
		res = setup_ncurses(&env);
	}
	if (res == RESULT_OK) {
		// print_board_complete(&board);
		// ft_printf("\n");
		// res = prompt_game_mode(&board.game_mode); // TODO Popup
		board.game_mode = LAST_LOSES;
	}
	if (res == RESULT_OK) {
		calc_finishers(&board);
		res = game_loop(&board, &env);
	}

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
		ft_dprintf(STDERR_FILENO, "\n\n\n\nMIN TERMINAL SIZE: %dx%d\n", MIN_TERMINAL_WIDTH, MIN_TERMINAL_HEIGTH);
		break;
	}

	cleanup_ncruses(&env);
	free_board(&board);
	return res;
}

size_t capped_sub(size_t a, size_t b)
{
	return a > b ? a - b : 0;
}

static void	turn(t_board *board, t_ncurses *env, Player *player)
{
	if (is_game_end(board)) {
		return;
	}

	t_row *cur_row = board->rows[board->cur_row];
	unsigned int picks = 0;
						
	if (*player == PLAYER) {
		picks = env->input.scroll_offset + 1;
	} else {
		picks = ai_pick(board);
	}
	cur_row->cur_amount -= picks;
	cur_row->last_pick = *player;
	board->picks[board->cur_turn] = picks;

	if (env->is_history) 
		update_history(env->history, board, capped_sub(board->cur_turn, env->history.size.y - 4));
	if (cur_row->cur_amount == 0 && board->cur_row != 0) {
		board->cur_row--;
	}
	board->cur_turn++;
	*player *= -1;
	update_board(&env->board, board, 0);
}

void update_history(t_win history, t_board *board, int offset) {
	werase(history.win);
	box(history.win, 0, 0);
	mvwprintw(history.win, 1, 1, "History:");
	int y_offset = 2;

	for (unsigned int i = offset; i <= board->cur_turn; i++) {
		mvwprintw(history.win, y_offset++, 1, "%s picked %d", 
			i % 2 == 0 ? "AI" : "You",
			board->picks[i]); // offset does NOT work with scrolling
				// either fix the capped_sub below or remove scrolling
			//capped_sub(board->cur_turn, history.size.y  - i)]);
	}
	wrefresh(history.win);
}

// void update_board(t_win *board_win, t_board *board) {
// 	werase(board_win->win);
// 	box(board_win->win, 0, 0);
// 	//something
// 	(void)board;
// 	wrefresh(board_win->win);
// }

void update_input(t_win input, t_board *board) {

	if (is_game_end(board)) {
		werase(input.win);
		box(input.win, 0, 0);
		//if mode is LAST_LOSES, the last player to pick wins
		mvwprintw(input.win, 1, 1, "%s won!", board->rows[board->cur_row]->last_pick == AI ? "You" : "AI");
		wrefresh(input.win);
		return;
	}
    const char *options[3] = {
        "1 pick",
        "2 picks",
        "3 picks"
    };

    werase(input.win); // could may be removed
    box(input.win, 0, 0); // same
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

WINDOW *detect_window(t_ncurses *env, unsigned int y, unsigned int x) {
	if (y >= env->history.pos.y && y < env->history.pos.y + env->history.size.y - 1
		&& x >= env->history.pos.x && x < env->history.pos.x + env->history.size.x - 1)
		return env->history.win;
	if (y >= env->input.pos.y && y < env->input.pos.y + env->input.size.y - 1
		&& x >= env->input.pos.x && x < env->input.pos.x + env->input.size.x - 1)
		return env->input.win;
	return env->board.win;
}

void mouse(t_ncurses *env, t_board* board)
{
	MEVENT event;
	if (getmouse(&event) == OK) {
		WINDOW *win = env->board.win;// detect_window(env, event.y, event.x);
		if (event.bstate & BUTTON5_PRESSED) //scrolldown
		{
			if (win == env->board.win && env->board.scroll_offset > 0)
			{
					env->board.scroll_offset--;
					update_board(&env->board, board, env->board.scroll_offset);
			}
			/* else if (win == env->history.win)
			{
				env->history.scroll_offset--;
				update_history(env->history, board, env->history.scroll_offset);
			}
			else if (win == env->input.win)
			{
				env->input.scroll_offset = (env->input.scroll_offset - 1 + board->num_options) % board->num_options;
			} */
		} else if (event.bstate & BUTTON4_PRESSED) //scrollup
		{
			if (win == env->board.win && board->cur_row > env->board.size.y - 3
				&& env->board.scroll_offset < board->cur_row - env->board.size.y + 3)
			{
					env->board.scroll_offset++;
					update_board(&env->board, board, env->board.scroll_offset);
			}
			/* else if (win == env->history.win)
			{
				env->history.scroll_offset++;
				update_history(env->history, board, env->history.scroll_offset);
			}
			else if (win == env->input.win)
			{
				env->input.scroll_offset = (env->input.scroll_offset + 1 + board->num_options) % board->num_options;
			} */
		}
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
	while (true)
	{
		int ch = wgetch(env->input.win);

		if (ch == '\n' && !is_game_end(board))
		{
			turn(board, env, &player);
			turn(board, env, &player);
			board->num_options = MIN(board->rows[board->cur_row]->cur_amount, 3);
			env->input.scroll_offset = 0;
			update_input(env->input, board);
		}
		else if (ch == 'q' || ch == 27) 
			break;
		else if (ch == KEY_UP && !is_game_end(board)) {
			env->input.scroll_offset = (env->input.scroll_offset - 1 + board->num_options) % board->num_options;
			update_input(env->input, board);
		} else if (ch == KEY_DOWN && !is_game_end(board)) {
			env->input.scroll_offset = (env->input.scroll_offset + 1) % board->num_options;
			update_input(env->input, board);
		} else if (ch == KEY_MOUSE) {
			mouse(env, board);
		}
	}
	return res;
}

static bool is_game_end(t_board *board)
{
	return board->cur_row == 0 && board->rows[board->cur_row]->cur_amount == 0;
}

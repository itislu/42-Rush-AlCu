#include "alcu.h"
#include "libft.h"
#include "visuals.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

// static void	test_draw(t_ncurses *env)
// {
// 	// sample text inside all windows
// 	mvwprintw(env->board.win, 1, 1, "Board");
// 	mvwprintw(env->history.win, 1, 1, "History");
// 	mvwprintw(env->input.win, 1, 1, "Input");
// 	// draw the windows
// 	wrefresh(env->board.win);
// 	wrefresh(env->history.win);
// 	wrefresh(env->input.win);

// }

// void	handle_input_window(t_ncurses *env, int key)
// {
// 	// get the next move thing
// }

// void	handle_history_window(t_ncurses *env, int key)
// {
// 	// update print from the board
// }

void update_board(t_win *n_board, t_board *board, int offset)
{
	int xoffset = 1;
	int yoffset = 1;
	int char_limit = 0;
	int text_len = 0;
	size_t i = 0;
	t_row *row = NULL;

	werase(n_board->win);
	box(n_board->win, 0, 0);
	if (board->cur_row - offset > n_board->size.y - 3) { // from - 1
		i = board->cur_row - offset - n_board->size.y + 3;
	}
	// while (i <= board->cur_row) {
	while (i <= board->cur_row - offset) {
		xoffset = 1;
		row = board->rows[i];
		char_limit = n_board->size.x - 2;
		text_len = ft_min(ft_nbrlen_base(i, 10) + 1, 3); // digits row '#123'
		text_len += ft_nbrlen_base(row->cur_amount, 10) + 4; // ' 123: '
		char_limit -= ft_min(row->cur_amount, char_limit - text_len);
		mvwprintw(n_board->win,
		          yoffset,
		          xoffset,
		          "#%-2zu %i:",
		          i + 1,
		          row->cur_amount);
		xoffset = n_board->size.x - 2;
		while (xoffset > char_limit) {
			mvwprintw(n_board->win, yoffset, xoffset--, "|");
		}
		yoffset++;
		i++;
	}
	wrefresh(n_board->win);
}

// void	handle_mouse_event(t_ncurses *env, int key, MEVENT *mouse)
// {
// 	// scrolling for each window
// }

// void	run_visuals(t_board *board)
// Result	run_visuals(t_board *board)
// {
// 	t_ncurses	env;
// 	// MEVENT		mouse;
// 	int key = 0;

// 	setup_ncurses(&env);
// 	// inital draw windows?
// 	test_draw(&env);
// 	while (key != 'q' && key != 27)
// 	{
// 		// input window
// 		// handle_input_window(&env, key);
// 		// history window
// 		// handle_history_window(&env, key);
// 		// board window
// 		update_board(&env.board, board);
// 		// if (key == KEY_MOUSE) {
// 		// 	getmouse(&mouse);
// 		// 	handle_mouse_event(&env, key, &mouse);
// 		// }
// 		key = wgetch(env.input.win);
// 	}
// 	cleanup_ncruses(&env);
// 	return (RESULT_OK);
// }

// int main(void)
// {
// 	run_visuals();
// 	return (0);
// }

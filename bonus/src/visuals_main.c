#include "visuals.h"
#include "ft_printf.h"
#include "libft.h"
// here to shutup the blue squiggles...
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

static void	test_draw(t_ncurses *env)
{
	// sample text inside all windows
	mvwprintw(env->board.win, 1, 1, "Board");
	mvwprintw(env->history.win, 1, 1, "History");
	mvwprintw(env->input.win, 1, 1, "Input");
	// draw the windows
	wrefresh(env->board.win);
	wrefresh(env->history.win);
	wrefresh(env->input.win);
	
}

// void	handle_input_window(t_ncurses *env, int key)
// {
// 	// get the next move thing
// }

// void	handle_history_window(t_ncurses *env, int key)
// {
// 	// update print from the board
// }

void	update_board(t_win *n_board, t_board *board)
{
	int	yoffset, xoffset;
	int char_limit;
	int text_len;
	t_row *row; //ft_snprintf

	for (size_t i = 0; i < board->height; i++) {
		xoffset = 1;
		yoffset = 1 + i;
		row = board->rows[i];
		char_limit = n_board->size.x - 2;
		text_len = ft_min(ft_nbrlen_base(i, 10) + 1, 3); // digits row '#123'
		text_len += ft_nbrlen_base(row->cur_amount, 10) + 4; // ' 123: '
		// char_limit = ft_min(text_len + row->cur_amount, char_limit - text_len);
		// char_limit -= text_len;
		char_limit -= ft_min(row->cur_amount, char_limit - text_len);
			// ft_min(char_limit - text_len);
		if (yoffset != 0 && yoffset != (getmaxy(n_board->win) - 1)) {
			mvwprintw(n_board->win, yoffset, xoffset, "#%-2zu %i:",
				i, row->cur_amount);
			// xoffset += text_len;
			xoffset = n_board->size.x - 2;
			while (xoffset > char_limit)
				mvwprintw(n_board->win, yoffset, xoffset--, "|");
				// mvwprintw(n_board->win, yoffset, xoffset++, "|")
		}
		// sleep(1);
			// mvwprintw(env->board.win, yoffset, 2, " line %zu", i);
	}
	wrefresh(n_board->win);
	// ft_printf("board window size (%i,%i)\n", env->board.size.x, env->board.size.y);
}

// void	handle_mouse_event(t_ncurses *env, int key, MEVENT *mouse)
// {
// 	// scrolling for each window
// }

// void	run_visuals(t_board *board)
Result	run_visuals(t_board *board)
{
	t_ncurses	env;
	// MEVENT		mouse;
	int key = 0;

	setup_ncurses(&env);
	// inital draw windows?
	test_draw(&env);
	while (key != 'q' && key != 27)
	{
		// input window
		// handle_input_window(&env, key);
		// history window
		// handle_history_window(&env, key);
		// board window
		update_board(&env.board, board);
		// if (key == KEY_MOUSE) {
		// 	getmouse(&mouse);
		// 	handle_mouse_event(&env, key, &mouse);
		// }
		key = wgetch(env.input.win);
	}
	cleanup_ncruses(&env);
	return (RES_OK);
}

// int main(void)
// {
// 	run_visuals();
// 	return (0);
// }

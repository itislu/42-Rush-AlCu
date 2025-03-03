#include "alcu.h"
#include "libft.h"
#include "visuals.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

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
	unsigned int xoffset = 1;
	unsigned int yoffset = 1;
	unsigned int text_len = 0;
	unsigned int stop_x = 0;
	unsigned int max_char_x = 0;
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
		// get max character that find in the windows
		max_char_x = n_board->size.x - 2; // -1 for borders R, -1 for index 0
		// calculate length of leading text
		text_len = ft_max(ft_nbrlen_base(i, 10) + 1, 3); // '#123' min:'#1 '
		text_len += ft_nbrlen_base(row->cur_amount, 10) + 4; // '  123: '
		// calculate  maximal available space for | characters
		stop_x = max_char_x - ft_min(row->cur_amount, max_char_x - text_len);
		// print leading text
		mvwprintw(n_board->win, yoffset, xoffset,
			"#%-2zu  %i:", i + 1, row->cur_amount);
		// adjustment for too many pieces in row
		if (row->cur_amount > max_char_x - text_len) {
			stop_x += 9; // offset for number of filler characters	
			mvwprintw(n_board->win, yoffset, xoffset + text_len, "|||| ... ");
		}
		// begin '|' print at max possible X coordinate
		// xoffset = max_char_x;
		// while (xoffset > stop_x) {
		// alternative approach from left to right
		xoffset += stop_x;
		while (xoffset < max_char_x) {
			mvwprintw(n_board->win, yoffset, xoffset++, "|");
		}
		yoffset++;
		i++;
	}
	wrefresh(n_board->win);
}

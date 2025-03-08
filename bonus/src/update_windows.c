#include "alcu.h"
#include "libft.h"
#include "visuals.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/param.h>

void update_history(t_win *history, t_board *board)
{
	int y_offset = 2;
	size_t start = 0;

	werase(history->win);
	box(history->win, 0, 0);
	mvwprintw(history->win, 1, 1, "History:");
	if (board->cur_turn - history->scroll_offset > history->size.y - 4) {
		start = board->cur_turn - history->scroll_offset - history->size.y + 4;
	}
	if (start && !history->scroll_offset && board->cur_turn % 2)
		start--;
	for (size_t i = start; i <= board->cur_turn - history->scroll_offset;
	     i++) {
		if (board->picks[i])
			mvwprintw(history->win,
					y_offset++,
					1,
					"#%zu %s picked %u",
					i + 1,
					i % 2 == 0 ? "AI" : "You",
					board->picks[i]);
	}
	wrefresh(history->win);
}

void update_input(t_win *input, t_board *board)
{
	if (board->game_mode == 0) {
		return;
	}
	if (is_game_end(board)) {
		print_res(input, board);
		return;
	}
	const char *options[3] = {"1 pick", "2 picks", "3 picks"};
	int offset = 3; // borders top+bottom, text line
	werase(input->win);
	box(input->win, 0, 0);
	if (input->size.x > 39)
		mvwprintw(input->win, 1, 1, "How many picks do you want to remove:"); // 37
	else {
		mvwprintw(input->win, 1, 1, "How many picks do"); // 17
		mvwprintw(input->win, 2, 1, "you want to remove:"); // 19
		offset++;
	}
	for (unsigned int i = 0; i < board->num_options; i++) {
		if (i == input->scroll_offset) {
			wattron(input->win, A_REVERSE);
		}
		mvwprintw(input->win, offset + i, 2, "%s", options[i]);
		wattroff(input->win, A_REVERSE);
	}
	wrefresh(input->win);
}

void update_board(t_win *n_board, t_board *board)
{
	unsigned int text_len = 0;
	unsigned int stop_x = 0;
	unsigned int max_char_x = 0;
	t_size offset;
	size_t i = 0;
	t_row *row = NULL;

	offset.x = 1;
	offset.y = 1;
	werase(n_board->win);
	box(n_board->win, 0, 0);
	// size.y - 3 = ncurses board windows height - border top & bottom - 1
	// - 1 = height starts at 1 instead of 0
	if (board->cur_row - n_board->scroll_offset > n_board->size.y - 3) {
		i = board->cur_row - n_board->scroll_offset - n_board->size.y + 3;
	}
	while (i <= board->cur_row - n_board->scroll_offset) {
		offset.x = 1;
		row = board->rows[i];
		// get max character that find in the windows
		max_char_x = n_board->size.x - 2; // -1 for borders R, -1 for index 0
		// calculate length of leading text
		text_len = ft_max(ft_nbrlen_base(i + 1, 10) + 1, 2); // '#123' min:'#1'
		text_len += ft_nbrlen_base(row->cur_amount, 10) + 5; // ' (123): '
		// calculate  maximal available space for | characters
		stop_x = max_char_x - ft_min(row->cur_amount, max_char_x - text_len);
		// print leading text
		mvwprintw(n_board->win, offset.y, offset.x,
			"#%-zu (%u):", i + 1, row->cur_amount);
		// adjustment for too many pieces in row
		if (row->cur_amount > max_char_x - text_len) {
			stop_x += 9; // offset for number of filler characters	
			mvwprintw(n_board->win, offset.y, offset.x + text_len, "|||| ... ");
		}
		// print | from left to right
		offset.x += stop_x;
		while (offset.x <= max_char_x) {
			mvwprintw(n_board->win, offset.y, offset.x++, "|");
		}
		offset.y++;
		i++;
	}
	wrefresh(n_board->win);	
}

void print_res(t_win *input, t_board *board)
{
	Player winner = AI;

	if ((board->game_mode == LAST_WINS && board->rows[0]->last_pick == PLAYER)
	    || (board->game_mode == LAST_LOSES
	        && board->rows[0]->last_pick == AI)) {
		winner = PLAYER;
	}

	werase(input->win);
	box(input->win, 0, 0);
	mvwprintw(input->win,
	          1,
	          1,
	          "%s",
	          winner == PLAYER ? "Congrats, you won!" : "You lost...");
	wrefresh(input->win);
}

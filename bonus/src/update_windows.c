#include "alcu.h"
#include "libft.h"
#include "visuals.h"
#include <ncurses.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/param.h>
#include <unistd.h>

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
		          "#%u %s picked %u",
		          i + 1,
		          i % 2 == 0 ? "AI" : "You",
		          board->picks[i]); // offset does NOT work with scrolling
		                            // either fix the capped_sub below or remove
		                            // scrolling
		// capped_sub(board->cur_turn, history->size.y  - i)]);
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

	werase(input->win);
	box(input->win, 0, 0);
	mvwprintw(input->win, 1, 1, "Select how many picks you want to remove:");

	for (unsigned int i = 0; i < board->num_options; i++) {
		if (i == input->scroll_offset) {
			wattron(input->win, A_REVERSE);
		}
		mvwprintw(input->win, 3 + i, 2, "%s", options[i]);
		wattroff(input->win, A_REVERSE);
	}
	wrefresh(input->win);
}

void update_board(t_win *n_board, t_board *board)
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
	if (board->cur_row - n_board->scroll_offset
	    > n_board->size.y - 3) { // from - 1
		i = board->cur_row - n_board->scroll_offset - n_board->size.y + 3;
	}
	// while (i <= board->cur_row) {
	while (i <= board->cur_row - n_board->scroll_offset) {
		xoffset = 1;
		row = board->rows[i];
		// get max character that find in the windows
		max_char_x = n_board->size.x - 2; // -1 for borders R, -1 for index 0
		// calculate length of leading text
		text_len = ft_max(ft_nbrlen_base(i, 10) + 1, 3);     // '#123' min:'#1 '
		text_len += ft_nbrlen_base(row->cur_amount, 10) + 4; // '  123: '
		// calculate  maximal available space for | characters
		stop_x = max_char_x - ft_min(row->cur_amount, max_char_x - text_len);
		// print leading text
		mvwprintw(n_board->win,
		          yoffset,
		          xoffset,
		          "#%-2zu  %u:",
		          i + 1,
		          row->cur_amount);
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
		while (xoffset <= max_char_x) {
			mvwprintw(n_board->win, yoffset, xoffset++, "|");
		}
		yoffset++;
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

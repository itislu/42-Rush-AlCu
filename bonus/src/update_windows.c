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

	print_title(history, "History");
	// offset *= -1;
	// offset += capped_sub(board->cur_turn, history->size.y - 4);
	// start = capped_sub(board->cur_turn, history->size.y - 4) - offset;
	if (board->cur_turn - history->scroll_offset > history->size.y - 1 - y_offset) {
		start = board->cur_turn - history->scroll_offset - history->size.y + 2 + y_offset;
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

	print_title(input, SELECT_PICKS);

	for (unsigned int i = 0; i < board->num_options; i++) {
		if (i == input->scroll_offset) {
			wattron(input->win, A_REVERSE);
		}
		mvwprintw(input->win, input->title_height + 1 + i, 2, "%s", options[i]);
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

	switch (board->game_mode) {
	case LAST_WINS:
		print_title(n_board, LAST_TO_PICK_WINS);
		break;
	case LAST_LOSES:
		print_title(n_board, LAST_TO_PICK_LOSES);
		break;
	default:
		print_title(n_board, NO_GAME_MODE);
	}
	offset.x = 1;
	offset.y = n_board->title_height;

	// size.y - 3 = ncurses board windows height - border top & bottom - 1
	// - 1 = height starts at 1 instead of 0
	if (board->cur_row - n_board->scroll_offset > n_board->size.y - 1 - offset.y) {
		i = board->cur_row - n_board->scroll_offset - n_board->size.y + 2 + offset.y;
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
	          winner == PLAYER ? WIN : LOSE);
	wrefresh(input->win);
}

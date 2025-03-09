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
	print_title(history, "History");
	
	size_t start = 0;
	int y_offset = history->title_height;

	if (board->cur_turn - history->scroll_offset > history->size.y - 2 - y_offset) {
		start = board->cur_turn - history->scroll_offset - history->size.y + 2 + y_offset;
	}

	for (size_t i = start; i <= board->cur_turn - history->scroll_offset; i++) {
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

	t_size offset;
	t_row *row = NULL;
	
	size_t start = 0;
	offset.y = n_board->title_height;

	if (board->cur_row - n_board->scroll_offset > n_board->size.y - 2 - offset.y) {
		start = board->cur_row - n_board->scroll_offset - n_board->size.y + 2 + offset.y;
	}
	for (size_t i = start; i <= board->cur_row - n_board->scroll_offset; i++) {
		offset.x = 1;
		row = board->rows[i];
		unsigned int max_char_x = n_board->size.x - 2;
		unsigned int text_len = ft_max(ft_nbrlen_base(i + 1, 10) + 1, 2);
		text_len += ft_nbrlen_base(row->cur_amount, 10) + 5;
		unsigned int stop_x = max_char_x - ft_min(row->cur_amount, max_char_x - text_len);
		mvwprintw(n_board->win, offset.y, offset.x,
			"#%-zu (%u):", i + 1, row->cur_amount);
		if (row->cur_amount > max_char_x - text_len) {
			stop_x += 9; // offset for number of filler characters	
			mvwprintw(n_board->win, offset.y, offset.x + text_len, "|||| ... ");
		}
		offset.x += stop_x;
		while (offset.x <= max_char_x) {
			mvwprintw(n_board->win, offset.y, offset.x++, "|");
		}
		offset.y++;
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
	print_title(input, winner == PLAYER ? WIN : LOSE);
}

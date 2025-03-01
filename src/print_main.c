#include "board.h"
#include "ft_printf.h"
#include "libft.h"

// static void print_clear_changed_row(t_board *board)
// {
// 	static size_t cur_row;
// 	static size_t delta;

// 	// --- initialization
// 	if (cur_row < board->cur_row) {
// 		if (board->height - board->cur_row == 1) {
// 			cur_row = board->cur_row;
// 		}
// 		else {
// 			cur_row = board->cur_row + 1;
// 		}
// 	}
// 	delta = cur_row - board->cur_row + 1;
// 	cur_row = board->cur_row;
// 	// --- clear rows above
// 	// clear_rows(delta + INPUT_OFFSET);
// 	clear_rows(delta);
// }

static void print_clear_complete_board(t_board *board, int player_offset)
{
	size_t curr_height;
	size_t offset = 1;
	
	static bool limit;
	
	curr_height = board->cur_row;

	// this is needed for correct update on HEIGHT limit
	if (board->cur_row > PRT_ROW_LIMIT) {
		offset++;
		limit = true;
	}
	else if (limit) {
		offset++;
		limit = false;
	}
	// removes one more line when the last line is empty
	
	curr_height = ft_min(curr_height, PRT_ROW_LIMIT);
	clear_rows(curr_height + offset + player_offset);
}

void print_board_gameloop(t_board *board, unsigned int pieces)
{
	int	player_offset = 0;
	static size_t row_prev_turn;
	static bool	init_done;

	// first inital print and previous row setup
	if (!init_done) {
		row_prev_turn = board->cur_row;
		print_board_complete(board);
		ft_printf("\nAI removed %i pieces\n", pieces);
		init_done = true;
		return ;
	}
	// determine the correct offset (what is printed after the board)
	Player cur_player = board->rows[board->cur_row]->last_pick;
	if (cur_player == PLAYER) {
		player_offset = PLAYER_OFFSET;
	}
	// offset change when line is finished
	if (row_prev_turn != board->cur_row) {
		player_offset++;
		if (board->rows[board->cur_row]->last_pick == PLAYER)
			row_prev_turn = board->cur_row;
	}
	// --- to remove all at the end?
	// if (!board->cur_row && !board->rows[board->cur_row]->cur_amount)
	// 	ft_printf("\033c");
	// else
	// determine whether to update the last row or the whole board
	// -- something with current row and max rows
	// -- print the board or the row
	print_clear_complete_board(board, player_offset);
	// ft_printf("current player_offset %i\n", player_offset);
	print_board_complete(board);
	if (cur_player == AI)
		ft_printf("\nAI removed %i pieces\n", pieces);
	// make this work
}

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
	static size_t last_row;
	static bool limit;
	
	curr_height = board->cur_row;
	if (!last_row && curr_height)
		last_row = curr_height;
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
	if (last_row != curr_height) {
		offset++;
		last_row = curr_height;
	}
	curr_height = ft_min(curr_height, PRT_ROW_LIMIT);
	clear_rows(curr_height + offset + player_offset);
}

void print_board_gameloop(t_board *board, unsigned int pieces)
{
	int	player_offset;
	// static size_t last_row;

	Player cur_player = board->rows[board->cur_row]->last_pick;
	if (cur_player == AI) {
		// ft_printf("AI removed %i pieces\n");
		player_offset = AI_OFFSET;
	}
	else {
		player_offset = PLAYER_OFFSET;
	}
	// --- to remove all at the end?
	// if (!board->cur_row && !board->rows[board->cur_row]->cur_amount)
	// 	ft_printf("\033c");
	// else
	print_clear_complete_board(board, player_offset);
	print_board_complete(board);
	if (cur_player == AI)
		ft_printf("\nAI removed %i pieces\n", pieces);
	// make this work
}

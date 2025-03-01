#include "board.h"
#include "ft_printf.h"
#include "libft.h"

static void print_board_row(t_board *board, size_t row);
static void clear_complete_board(t_board *board, int player_offset);

void print_board_gameloop(t_board *board, unsigned int pieces)
{
	int	player_offset = 0;
	static size_t row_prev_turn;
	static bool	init_done;
	bool row_change = false;

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
		row_change = true;
		player_offset++;
		if (board->rows[board->cur_row]->last_pick == PLAYER)
			row_prev_turn = board->cur_row;
	}
	// replace only last line or the whole board
	if (!row_change) {
		// print_clear_changed_row(board, player_offset);
		clear_rows(player_offset + 1);
		print_board_row(board, board->cur_row);
		// ft_printf("it worked!\n");
	}
	else {
		clear_complete_board(board, player_offset);
		print_board_complete(board);
	}
	if (cur_player == AI)
		ft_printf("\nAI removed %i pieces\n", pieces);
}


static void print_board_row(t_board *board, size_t row)
{
	unsigned int spaces;
	unsigned int pieces;
	unsigned int width;

	pieces = board->rows[row]->cur_amount;
	width = ft_min(PRT_WIDTH_LIMIT, board->width);
	if (pieces <= PRT_WIDTH_LIMIT) {
		spaces = (width / 2 - pieces / 2) * 2;
		while (spaces--) {
			ft_printf(" ");
		}
		spaces = (width * 2)                      // total = max monospace chars
		         - ((width / 2 - pieces / 2) * 2) // - spaces before the pins
		         - (pieces * 2) + 2;              // - pins
		while (pieces--) {
			ft_printf("%s", PIECES);
		}
	}
	else {
		for (int i = 0; i < PRT_PIN_FILLER; i++) {
			ft_printf("%s", PIECES);
		}
		// this is now adjusted to 3 filler pins left and right
		ft_printf("   ...      %5i pieces left     ...   ", pieces);
		for (int i = 0; i < PRT_PIN_FILLER; i++) {
			ft_printf("%s", PIECES);
		}
		spaces = 2;
	}
	while (--spaces) {
		ft_printf(" ");
	}
	ft_printf(" row #%i (%i pieces)\n", row + 1, board->rows[row]->cur_amount);
}

void print_board_complete(t_board *board)
{
	size_t row;
	unsigned int width;

	width = ft_min(PRT_WIDTH_LIMIT, board->width);
	if (board->cur_row > PRT_ROW_LIMIT) {
		row = board->cur_row - PRT_ROW_LIMIT;
		ft_printf("   ");
		width /= 2;
		while (width--) {
			ft_printf(" ");
		}
		ft_printf("there are %i more rows above\n", row);
	}
	else {
		row = 0;
	}
	while (row < board->cur_row + 1) {
		print_board_row(board, row++);
	}
}

static void clear_complete_board(t_board *board, int player_offset)
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

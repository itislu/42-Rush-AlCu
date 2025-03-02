#include "alcu.h"
#include "ft_printf.h"
#include "libft.h"
#include "print.h"
#include <stddef.h>

static void print_board_row(t_board *board, size_t row);
static void clear_complete_board(t_board *board, int offset);

void print_board_gameloop(t_board *board, unsigned int pieces)
{
	static size_t row_prev_turn;
	static bool init_done;
	int offset = 0;
	bool row_change = false;
	static unsigned int	player_pieces;

	Player cur_player = board->rows[board->cur_row]->last_pick;
	if (cur_player == PLAYER)
		player_pieces = pieces;
	// first inital print and previous row setup
	if (!init_done) {
		row_prev_turn = board->cur_row;
		print_board_complete(board);
		ft_printf("\nAI removed %i%s\n\n", pieces, PRT_PIECE);
		init_done = true;
		return;
	}
	// determine the correct offset (what is printed after the board)
	if (cur_player == PLAYER) {
		offset = PRT_OFFSET;
	}
	// offset change when line is finished
	if (row_prev_turn != board->cur_row) {
		offset++;
		row_change = true;
		row_prev_turn = board->cur_row;
	}
	// replace only last line or the whole board
	if (!row_change) {
		clear_rows(offset + 1);
		print_board_row(board, board->cur_row);
	}
	else {
		clear_complete_board(board, offset);
		print_board_complete(board);
	}
	if (cur_player == AI) {
		ft_printf("\nYou removed %i%s :: AI removed %i%s\n\n",
			player_pieces, PRT_PIECE, pieces, PRT_PIECE);
	}
	// adjustment for last player move
	else if (board->cur_row == 0 && board->rows[0]->cur_amount == 0) {
		ft_printf("\nYou removed %i%s\n\n", player_pieces, PRT_PIECE);
	}
}

static void print_board_row(t_board *board, size_t row)
{
	unsigned int spaces = 0;
	unsigned int pieces = board->rows[row]->cur_amount;
	unsigned int width = ft_min(PRT_WIDTH_LIMIT, board->width);

	if (pieces <= PRT_WIDTH_LIMIT) {
		spaces = (width / 2 - pieces / 2) * 2;
		while (spaces--) {
			ft_printf(" ");
		}
		spaces = (width * 2)                      // total = max monospace chars
		         - ((width / 2 - pieces / 2) * 2) // - spaces before the pins
		         - (pieces * 2) + 2;              // - pins
		while (pieces--) {
			ft_printf("%s", PRT_PIECE);
		}
	}
	else {
		for (int i = 0; i < PRT_PIN_FILLER; i++) {
			ft_printf("%s", PRT_PIECE);
		}
		ft_printf(" ...%5i%s ... ", pieces, PRT_PIECE);
		for (int i = 0; i < PRT_PIN_FILLER; i++) {
			ft_printf("%s", PRT_PIECE);
		}
		spaces = 2;
	}
	while (--spaces) {
		ft_printf(" ");
	}
	ft_printf(
	    "    #%-3i %5i%s\n", row + 1, board->rows[row]->cur_amount, PRT_PIECE);
}

void print_board_complete(t_board *board)
{
	size_t row = 0;
	unsigned int width = ft_min(PRT_WIDTH_LIMIT, board->width);

	if (board->cur_row > PRT_ROW_LIMIT) {
		row = board->cur_row - PRT_ROW_LIMIT;
		width -= 14; // half the length of the string to print
		while (width--) {
			ft_printf(" ");
		}
		ft_printf("... %3i more heaps above ...\n", row);
	}
	while (row < board->cur_row + 1) {
		print_board_row(board, row++);
	}
}

static void clear_complete_board(t_board *board, int offset_in)
{
	size_t curr_height = board->cur_row;
	size_t offset = 1;

	curr_height = ft_min(curr_height, PRT_ROW_LIMIT);
	clear_rows(curr_height + offset + offset_in);
}

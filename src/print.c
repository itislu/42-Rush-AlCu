#include "board.h"
#include "ft_printf.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

#define PIECES          "📍"
#define INPUT_OFFSET    3
#define PRT_ROW_LIMIT   10
#define PRT_WIDTH_LIMIT 26
#define AI_OFFSET		3
#define PLAYER_OFFSET	2
#define PRT_PIN_FILLER 3

void clear_rows(int rows);


void print_board_row(t_board *board, size_t row)
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
		ft_printf("     ");
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

void clear_complete_board(t_board *board, int player_offset)
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
	// clear_rows(curr_height + offset);
}

void clear_changed_row(t_board *board)
{
	static size_t cur_row;
	static size_t delta;

	// --- initialization
	if (cur_row < board->cur_row) {
		if (board->height - board->cur_row == 1) {
			cur_row = board->cur_row;
		}
		else {
			cur_row = board->cur_row + 1;
		}
	}
	delta = cur_row - board->cur_row + 1;
	cur_row = board->cur_row;
	// --- clear rows above
	// clear_rows(delta + INPUT_OFFSET);
	clear_rows(delta);
}

void print_board_gameloop(t_board *board, int pieces)
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
	clear_complete_board(board, player_offset);
	print_board_complete(board);
	if (cur_player == AI)
		ft_printf("\nAI removed %i pieces\n", pieces);
	// make this work
}

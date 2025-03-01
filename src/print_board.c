#include "board.h"
#include "ft_printf.h"
#include "libft.h"


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

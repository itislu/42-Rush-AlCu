#include "board.h"
#include "ft_printf.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

# define PIECES	"📍"
# define INPUT_OFFSET 3
# define PRT_ROW_LIMIT 10
# define PRT_WIDTH_LIMIT 26
# define PRT_PIN_FILLER 3

void clear_rows(int rows);

void	print_board_row(t_board *board, size_t row)
{
	unsigned int	spaces;
	unsigned int	pieces;
	unsigned int	width;

	pieces = board->rows[row].cur_amount;
	width = ft_min(PRT_WIDTH_LIMIT, board->width);
	if (pieces <= PRT_WIDTH_LIMIT)
	{
		spaces = (width / 2 - pieces / 2) * 2;
		while (spaces--)
			ft_printf(" ");
		spaces = (width * 2)					// total = max monospace chars
			- ((width / 2 - pieces / 2) * 2)	// - spaces before the pins
			- (pieces * 2) + 2;					// - pins
		while (pieces--)
			ft_printf("%s", PIECES);
	}
	else
	{
		for (int i = 0; i < PRT_PIN_FILLER; i++)
			ft_printf("%s", PIECES);
		// this is now adjusted to 3 filler pins left and right
		ft_printf("   ...      %5i pieces left     ...   ", pieces);
		for (int i = 0; i < PRT_PIN_FILLER; i++)
			ft_printf("%s", PIECES);
		spaces = 2;
	}
	while (--spaces)
		ft_printf(" ");
	ft_printf(" row #%i (%i pieces)\n", row + 1, board->rows[row].cur_amount);
}

void	print_board_complete(t_board *board)
{
	size_t			row;
	unsigned int	width;

	width = ft_min(PRT_WIDTH_LIMIT, board->width);
	if (board->cur_row > PRT_ROW_LIMIT)
	{
		row = board->cur_row - PRT_ROW_LIMIT;
		ft_printf("     ");
		width /= 2;
		while (width--)
			ft_printf(" ");
		ft_printf("there are %i more rows above\n", row);
	}
	else
		row = 0;
	while (row < board->cur_row + 1)
	{
		print_board_row(board, row++);
	}
}

void	clear_complete_board(t_board *board)
{
	size_t		row;
	size_t		curr_height;
	size_t		offset = 1;
	static bool	limit;

	curr_height = board->cur_row + 1;
	if (board->cur_row > PRT_ROW_LIMIT)
	{
		offset++;
		limit = true;
	}
	else if (limit)
	{
		offset++;
		limit = false;
	}
	curr_height = ft_min(curr_height, PRT_ROW_LIMIT);
	// clear_rows(curr_height + offset + INPUT_OFFSET);
	clear_rows(curr_height + offset);
}

void	clear_changed_row(t_board *board)
{
	static size_t	cur_row;
	static size_t	delta;

	// --- initialization
	if (cur_row < board->cur_row)
	{
		if (board->height - board->cur_row == 1)
			cur_row = board->cur_row;
		else
			cur_row = board->cur_row + 1;
	}
	delta = cur_row - board->cur_row + 1;
	cur_row = board->cur_row;
	// --- clear rows above
	// clear_rows(delta + INPUT_OFFSET);
	clear_rows(delta);
}

// static void	test_empty_row_keep_width(t_board *board, size_t row)
// {
// 	board->rows[row].cur_amount = 0;
// 	if (board->cur_row)
// 		board->cur_row--;
// 	usleep(50000);
// 	clear_complete_board(board);
// 	print_board_complete(board);
// }

// static void	test_set_width(t_board *board)
// {
// 	size_t	row = 0;

// 	while (row < board->height)
// 	{
// 		board->width = ft_max(board->width, board->rows[row++].cur_amount);
// 	}
// }


// int	main()
// {
// 	t_board	board;
// 	size_t	height = 15;

// 	// keeping width
// 	ft_memset(&board, 0, sizeof board);
// 	height = 70;
// 	board.rows = ft_calloc(height, sizeof (t_row));
// 	for (int i = 0; i < height; i++)
// 	{
// 		board.rows[i].cur_amount = (i + 1) * 3;
// 		board.rows[i].start_amount = (i + 1) * 3;
// 		board.height++;
// 	}
// 	board.cur_row = board.height - 1;
// 	test_set_width(&board);
// 	print_board_complete(&board);
// 	for (int i = 0; i < height; i++)
// 		test_empty_row_keep_width(&board, board.cur_row);
// 	free(board.rows);
// }

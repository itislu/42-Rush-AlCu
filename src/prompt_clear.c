#include "libft.h"

static void move_up_a_line()
{
	ft_putstr_fd("\033[A\r", 1);
}

// static void move_down_a_line()
// {
// 	ft_putstr_fd("\033[B\r", 1);
// }

static void clear_line()
{
	ft_putstr_fd("\033[K\r", 1);
}

void clear_rows(int rows)
{
	for (int i = 0; i < rows; i++)
	{
		move_up_a_line();
		clear_line();
	}
}

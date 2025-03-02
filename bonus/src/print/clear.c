#include "libft.h"
#include "print.h"

void clear_rows(int rows)
{
	for (int i = 0; i < rows; i++) {
		move_up_a_line();
		clear_line();
	}
}

void clear_line(void)
{
	ft_putstr_fd("\033[2K\r", 1);
}

void move_up_a_line(void)
{
	ft_putstr_fd("\033[A\r", 1);
}

void move_down_a_line(void)
{
	ft_putstr_fd("\033[B\r", 1);
}

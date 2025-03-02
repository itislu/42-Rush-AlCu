#include "ft_printf.h"
#include "libft.h"
#include "print.h"
#include <stddef.h>

void print_boxed(const char *str)
{
	print_boxed_specialstr(str, ft_strlen(str));
}

void print_boxed_specialstr(const char *str, size_t str_len)
{
	size_t box_width = str_len + 4;

	ft_printf("╭");
	for (size_t i = 0; i < box_width - 2; i++) {
		ft_printf("─");
	}
	ft_printf("╮\n");

	ft_printf("│ %s │\n", str);

	ft_printf("╰");
	for (size_t i = 0; i < box_width - 2; i++) {
		ft_printf("─");
	}
	ft_printf("╯\n");
}

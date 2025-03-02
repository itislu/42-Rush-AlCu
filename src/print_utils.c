#include "alcu.h"
#include "ft_printf.h"
#include "libft.h"
#include <stddef.h>

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

void print_boxed(const char *str)
{
	print_boxed_specialstr(str, ft_strlen(str));
}

void print_game_mode(Mode mode)
{
	const char *title = NULL;

	if (mode == LAST_LOSES) {
		title = "🚨 LAST PICK LOSES 🚨";
	}
	else {
		title = "🏅 LAST PICK WINS 🏅";
	}
	print_boxed_specialstr(title, ft_strlen(title) - 4);
}

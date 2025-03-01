#include "board.h"
#include "ft_printf.h"
#include "get_next_line.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

unsigned int prompt(t_board *board)
{
	char *line = NULL;
	unsigned int picks = 0;

	ft_printf("Make up a prompt message!!! Between 1-3");
	while (true)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			ft_printf("Bye-Bye! 👋");
			return 0;
		}
		picks = ft_atoi(line);
		if (is_valid_number(line) == false
		&& picks >= 1 && picks <= 3 && picks <= board->rows->cur_amount) {
			return (free(line), picks);
		}
		free(line);
		//clear 2 lines
		ft_printf("REPROMPT!!! Between 1-3");
	}
}

bool is_valid_number(const char *num)
{
	if (!num || ft_isoverflow_int(num)) {
		return (false);
	}
	int i = 0;
	while (num[i] && ft_isspace(num[i])) {
		i++;
	}
	i += num[i] == '+';
	while (num[i] && ft_isdigit(num[i])) {
		i++;
	}
	while (num[i] && ft_isspace(num[i])) {
		i++;
	}
	return (num[i] == '\0');
}

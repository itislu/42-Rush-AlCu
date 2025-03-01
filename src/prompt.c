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
	t_row *curr_row = board->rows[board->cur_row];

	ft_printf("Make up a prompt message!!! Between 1-3");
	while (true)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			ft_printf("Bye-Bye! 👋");
			return 0;
		}
		if (is_valid_number(line) == false) {
			return (free(line), false);
		}
		picks = ft_atoi(line);
		free(line);
		if (picks >= 1 && picks <= 3 && picks <= curr_row->cur_amount) {
			return (picks);
		}
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

bool prompt_game_mode(Mode *mode)
{
	char *line = NULL;
	int mod = 0;

	ft_printf("Select game mode!\n1	Last to pick loses\n2	Last to pick wins\n");
	while (true)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			ft_printf("Bye-Bye! 👋");
			return (false);
		}
		if (is_valid_number(line) == false) {
			return (free(line), false);
		}
		mod = ft_atoi(line);
		free(line);
		if (mod == 1 || mod == 2) {
			*mode = mod;
			return (true);
		}
		//clear 2 lines
		ft_printf("REPROMPT!!! 1 or 2");
	}
}

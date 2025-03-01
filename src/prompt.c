#include "board.h"
#include "ft_printf.h"
#include "get_next_line.h"
#include "libft.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

Result get_input(char **line)
{
	errno = 0;
	*line = get_next_line(STDIN_FILENO);
	if (!*line) {
		if (errno != 0) {
			return INTERNAL_ERROR;
		}
		return USER_EXIT;
	}
	return OK;
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

Result prompt_picks(t_board *board, unsigned int *picks)
{
	Result res = OK;
	char *line = NULL;
	t_row *curr_row = board->rows[board->cur_row];

	ft_printf("Make up a prompt message!!! Between 1-3");
	while (true) {
		res = get_input(&line);
		if (res != OK) {
			break;
		}
		if (is_valid_number(line)) {
			*picks = ft_atoi(line);
			if (*picks >= 1 && *picks <= 3 && *picks <= curr_row->cur_amount) {
				break;
			}
		}
		free(line);
		// clear 2 lines
		ft_printf("REPROMPT!!! Between 1-3");
	}
	free(line);
	return res;
}

Result prompt_game_mode(Mode *mode)
{
	Result res = OK;
	char *line = NULL;
	int mod = 0;

	ft_printf(
	    "Select game mode!\n1	Last to pick loses\n2	Last to pick wins\n");
	while (true) {
		res = get_input(&line);
		if (res != OK) {
			break;
		}
		if (is_valid_number(line)) {
			mod = ft_atoi(line);
			if (mod == 1 || mod == 2) {
				*mode = mod;
				break;
			}
		}
		free(line);
		// clear 2 lines
		ft_printf("REPROMPT!!! 1 or 2");
	}
	free(line);
	return res;
}

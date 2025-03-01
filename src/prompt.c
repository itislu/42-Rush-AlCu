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

bool is_valid_number(const char *num, unsigned int lower, unsigned int upper)
{
	if (!num || ft_isoverflow_int(num)) {
		return (false);
	}
	int i = 0;
	while (num[i] && ft_isspace(num[i])) {
		i++;
	}
	i += num[i] == '+';

	bool has_digit = false;
	while (num[i] && ft_isdigit(num[i])) {
		has_digit = true;
		i++;
	}
	if (!has_digit) {
		return false;
	}

	while (num[i] && ft_isspace(num[i])) {
		i++;
	}
	if (num[i] != '\0') {
		return false;
	}

	unsigned int number = (unsigned int)ft_atol(num);
	return (lower <= number && number <= upper);
}

Result prompt_picks(t_board *board, unsigned int *picks)
{
	Result res = OK;
	char *line = NULL;
	t_row *curr_row = board->rows[board->cur_row];

	ft_printf(
	    "Make up a prompt message!!! Between %i-%i", MIN_PICKS, MAX_PICKS);
	while (true) {
		res = get_input(&line);
		if (res != OK) {
			break;
		}
		if (is_valid_number(
		        line, MIN_PICKS, MIN(MAX_PICKS, curr_row->cur_amount))) {
			*picks = ft_atoi(line);
			break;
		}
		free(line);
		// clear 2 lines
		ft_printf("REPROMPT!!! Between %i-%i", MIN_PICKS, MAX_PICKS);
	}
	free(line);
	return res;
}

Result prompt_game_mode(Mode *mode)
{
	Result res = OK;
	char *line = NULL;

	ft_printf(
	    "Select game mode!\n%i	Last to pick loses\n%i	Last to pick wins\n",
	    LAST_WINS,
	    LAST_LOSES);
	while (true) {
		res = get_input(&line);
		if (res != OK) {
			break;
		}
		if (is_valid_number(line, LAST_WINS, LAST_LOSES)) {
			*mode = ft_atoi(line);
			break;
		}
		free(line);
		// clear 2 lines
		ft_printf("REPROMPT!!! %i or %i", LAST_WINS, LAST_LOSES);
	}
	free(line);
	return res;
}

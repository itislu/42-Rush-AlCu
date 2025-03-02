#include "board.h"
#include "ft_printf.h"
#include "get_next_line.h"
#include "libft.h"
#include "print_utils.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/param.h>
#include <unistd.h>

Result get_input(char **line, int fd)
{
	errno = 0;
	if (fd == STDIN_FILENO) {
		ft_printf("> ");
	}
	*line = get_next_line(fd);
	if (*line == NULL
	    || (fd == STDIN_FILENO && (*line)[ft_strlen(*line) - 1] != '\n')) {
		if (errno != 0) {
			return INTERNAL_ERROR;
		}
		if (fd == STDIN_FILENO) {
			return USER_EXIT;
		}
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
	unsigned int max_picks = MIN(MAX_PICKS, curr_row->cur_amount);
	bool toggle = false;

	while (true) {
		ft_printf("Pick up to %i%s:\n", max_picks, PIECE);
		res = get_input(&line, STDIN_FILENO);
		if (res != OK) {
			break;
		}
		if (is_valid_number(
		        line, MIN_PICKS, MIN(max_picks, curr_row->cur_amount))) {
			*picks = ft_atoi(line);
			break;
		}
		free(line);
		clear_rows(2);
		if (toggle) {
			ft_printf("Still invalid input. ");
		}
		else {
			ft_printf("Invalid input. ");
		}
		toggle ^= true;
	}
	free(line);
	return res;
}

Result prompt_game_mode(Mode *mode)
{
	Result res = OK;
	char *line = NULL;
	bool toggle = false;

	const char title[] = "📌 GAME MODE SELECTION 📌";
	print_boxed_specialstr(title, sizeof(title) - 1 - 4);
	while (true) {
		ft_printf(
		    "Select a game mode:\n%i - Last to pick loses\n%i - Last to pick wins\n",
		    LAST_LOSES,
		    LAST_WINS);
		res = get_input(&line, STDIN_FILENO);
		if (res != OK) {
			break;
		}
		if (is_valid_number(line, LAST_LOSES, LAST_WINS)) {
			*mode = ft_atoi(line);
			break;
		}
		free(line);
		clear_rows(4);
		if (toggle) {
			ft_printf("Still invalid input. ");
		}
		else {
			ft_printf("Invalid input. ");
		}
		toggle ^= true;
	}
	free(line);

	clear_rows(7);
	print_game_mode(*mode);
	ft_printf("\n");
	return res;
}

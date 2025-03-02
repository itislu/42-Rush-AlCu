#include "alcu.h"
#include "ft_printf.h"
#include "get_next_line.h"
#include "libft.h"
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

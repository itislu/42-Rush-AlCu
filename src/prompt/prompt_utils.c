#include "alcu.h"
#include "ft_printf.h"
#include "get_next_line.h"
#include "libft.h"
#include "prompt.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/param.h>
#include <unistd.h>

Result read_line_user(char **line, int fd)
{
	ft_printf("> ");
	Result res = read_line_file(line, fd);
	if (res != OK) {
		return res;
	}
	if (*line == NULL || (*line)[ft_strlen(*line) - 1] != '\n') {
		return USER_EXIT;
	}
	return res;
}

Result read_line_file(char **line, int fd)
{
	errno = 0;
	*line = get_next_line(fd);
	if (errno != 0) {
		return INTERNAL_ERROR;
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

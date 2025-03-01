#include "board.h"
#include "get_next_line.h"
#include "libft.h"
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static int open_file(const char *filename);
static bool is_input_end(const char *line);
static t_row *new_row(const char *line);

Result read_rows(t_list **rows, const char *filename)
{
	int fd = open_file(filename);
	if (fd == -1) {
		return INTERNAL_ERROR;
	}

	char *line = NULL;
	Result res = OK;

	errno = 0;
	while ((line = get_next_line(fd))) {
		if (is_input_end(line)) {
			break;
		}
		t_row *row = new_row(line);
		if (row == NULL) {
			res = BOARD_ERROR;
			break;
		}
		if (!ft_lstnew_back(rows, row)) {
			free(row);
			break;
		}
		free(line);
	}
	free(line);
	if (fd != STDIN_FILENO) {
		close(fd);
	}
	if (errno != 0) {
		res = INTERNAL_ERROR;
	}
	return res;
}

static int open_file(const char *filename)
{
	if (filename == NULL) {
		return STDIN_FILENO;
	}
	return open(filename, O_RDONLY);
}

static bool is_input_end(const char *line)
{
	return (line[0] == '\0');
}

static t_row *new_row(const char *line)
{
	if (!is_valid_number(line)) {
		return NULL;
	}
	t_row *row = ft_calloc(1, sizeof(t_row));
	if (row == NULL) {
		return NULL;
	}

	int nbr = ft_atoi(line);
	if (MIN_ROW_AMOUNT <= nbr && nbr <= MAX_ROW_AMOUNT) {
		free(row);
		return NULL;
	}
	row->start_amount = nbr;
	row->cur_amount = nbr;
	return row;
}

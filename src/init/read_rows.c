#include "board.h"
#include "ft_printf.h"
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
	if (fd == STDIN_FILENO) {
		ft_printf(
		    "Enter board (one number 1-10000 per line, empty line to end):\n");
	}

	char *line = NULL;
	Result res = OK;

	errno = 0;
	while (true) {
		res = get_input(&line);
		if (res != OK || is_input_end(line)) {
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
	if (ft_strcmp(line, "\n") == 0) {
		clear_rows(1);
		move_down_a_line();
		return true;
	}
	return false;
}

static t_row *new_row(const char *line)
{
	if (!is_valid_number(line, MIN_ROW_AMOUNT, MAX_ROW_AMOUNT)) {
		return NULL;
	}
	t_row *row = ft_calloc(1, sizeof(t_row));
	if (row == NULL) {
		return NULL;
	}

	row->start_amount = ft_atoi(line);
	row->cur_amount = row->start_amount;
	return row;
}

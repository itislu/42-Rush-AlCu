#include "alcu.h"
#include "ft_printf.h"
#include "get_next_line.h"
#include "libft.h"
#include "print.h"
#include "prompt.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static Result read_file(t_list **rows, int fd);
static bool is_input_end(const char *line);
static t_row *new_row(const char *line);

Result read_rows(t_list **rows, const char *filename)
{
	int fd = g_stdin;

	if (filename != NULL) {
		fd = open(filename, O_RDONLY);
		if (fd == -1) {
			return INTERNAL_ERROR;
		}
	}
	else {
		const char title[] = "🔨 BOARD CREATION 🔨";
		print_boxed_specialstr(title, sizeof(title) - 1 - 4);
		ft_printf(
		    "Enter board (one number 1-10000 per line, empty line to end):\n");
	}
	Result res = read_file(rows, fd);

	close(fd);
	if (fd != g_stdin) {
		close(g_stdin);
	}
	g_stdin = open("/dev/tty", O_RDONLY);
	if (g_stdin == -1) {
		res = INTERNAL_ERROR;
	}
	return res;
}

static Result read_file(t_list **rows, int fd)
{
	Result res = RESULT_OK;
	char *line = NULL;
	t_list *tail = NULL;

	while ((res = read_line(&line, fd)) == RESULT_OK) {
		if (is_input_end(line)) {
			if (fd == g_stdin) {
				clear_rows(1);
				move_down_a_line();
			}
			break;
		}
		if (!is_valid_number(line, MIN_ROW_AMOUNT, MAX_ROW_AMOUNT)) {
			res = BOARD_ERROR;
			break;
		}
		t_row *row = new_row(line);
		if (row == NULL || !ft_lstnew_back_eff(rows, &tail, row)) {
			free(row);
			res = INTERNAL_ERROR;
			break;
		}
		free(line);
	}
	free(line);
	free_get_next_line();
	return res;
}

static bool is_input_end(const char *line)
{
	return (ft_strcmp(line, "\n") == 0);
}

static t_row *new_row(const char *line)
{
	t_row *row = ft_calloc(1, sizeof(t_row));
	if (row == NULL) {
		return NULL;
	}
	row->start_amount = ft_atoi(line);
	row->cur_amount = row->start_amount;
	return row;
}

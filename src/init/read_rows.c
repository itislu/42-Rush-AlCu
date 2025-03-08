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

static Result
read_file(t_list **rows, int fd, Result (*read_line)(char **, int));
static bool is_input_end(const char *line);
static t_row *new_row(const char *line);
static bool is_interactive(void);
static Result make_interactive(void);

Result read_rows(t_list **rows, const char *filename)
{
	Result res = OK;
	const bool interactive = is_interactive();

	if (filename != NULL) {
		int fd = open(filename, O_RDONLY);
		if (fd == -1) {
			return INTERNAL_ERROR;
		}
		res = read_file(rows, fd, read_line_file);
		close(fd);
	}
	else if (!interactive) {
		res = read_file(rows, STDIN_FILENO, read_line_file);
	}
	else { // interactive
		const char title[] = "🔨 BOARD CREATION 🔨";
		print_boxed_specialstr(title, sizeof(title) - 1 - 4);
		ft_printf(
		    "Enter board (one number 1-10000 per line, empty line to end):\n");
		res = read_file(rows, STDIN_FILENO, read_line_user);
		if (res == OK) {
			clear_rows(1);
			move_down_a_line();
		}
	}

	if (!interactive) {
		res = make_interactive();
	}
	return res;
}

static Result
read_file(t_list **rows, int fd, Result (*read_line)(char **, int))
{
	Result res = OK;
	char *line = NULL;
	t_list *tail = NULL;

	while ((res = read_line(&line, fd)) == OK) {
		if (is_input_end(line)) {
			break;
		}
		t_row *row = new_row(line);
		if (row == NULL) {
			res = BOARD_ERROR;
			break;
		}
		if (!ft_lstnew_back_eff(rows, &tail, row)) {
			free(row);
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
	return (line == NULL || ft_strcmp(line, "\n") == 0);
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

/**
 * Detects if STDIN_FILENO is interactive by checking if its 'open-flags' are
 * 'O_RDWR | O_APPEND'.
 *
 * The fdinfo of a normal stdin looks like this:
 *     pos:    0
 *     flags:  02002
 *     mnt_id: 27
 *     ino:    17
 * A redirected stdin looks like this:
 *     pos:    0
 *     flags:  0100000
 *     mnt_id: 25
 *     ino:    23350703
 */
static bool is_interactive(void)
{
	const int info_fd = open("/proc/self/fdinfo/0", O_RDONLY);
	if (info_fd == -1) {
		return false;
	}

	bool res = false;
	char *line = NULL;
	while (read_line_file(&line, info_fd) == OK) {
		if (ft_strnstr(line, "flags:", ft_strlen(line)) != NULL) {
			const char *end = ft_strrchr(line, '2');
			if (end != NULL && !ft_isdigit(end[1]) && end[-3] == '2') {
				res = true;
			}
			break;
		}
		free(line);
	}
	free(line);
	free_get_next_line();
	close(info_fd);
	return res;
}

static Result make_interactive(void)
{
	close(STDIN_FILENO);
	if (open("/dev/tty", O_RDONLY) == -1) { // STDIN_FILENO gets closed at exit
		return INTERNAL_ERROR;
	}
	return OK;
}

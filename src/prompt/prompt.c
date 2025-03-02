#include "prompt.h"
#include "alcu.h"
#include "ft_printf.h"
#include "libft.h"
#include "print.h"
#include <stdlib.h>
#include <sys/param.h>
#include <unistd.h>

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

Result prompt_picks(t_board *board, unsigned int *picks)
{
	Result res = OK;
	char *line = NULL;
	t_row *curr_row = board->rows[board->cur_row];
	unsigned int max_picks = MIN(MAX_PICKS, curr_row->cur_amount);
	bool toggle = false;

	while (true) {
		ft_printf("Pick up to %i%s:\n", max_picks, PRT_PIECE);
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

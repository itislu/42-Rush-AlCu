#include "prompt.h"
#include "alcu.h"
#include "ft_printf.h"
#include "libft.h"
#include "print.h"
#include <stdlib.h>
#include <sys/param.h>
#include <unistd.h>

static Result prompt_number(unsigned int *number,
                            const char *prompt,
                            unsigned int min,
                            unsigned int max);

Result prompt_game_mode(Mode *mode)
{
	Result res = RESULT_OK;
	char prompt[100];

	const char title[] = "📌 GAME MODE SELECTION 📌";
	print_boxed_specialstr(title, sizeof(title) - 1 - 4);

	ft_snprintf(
	    prompt,
	    sizeof(prompt),
	    "Select a game mode:\n%i - Last to pick loses\n%i - Last to pick wins\n",
	    LAST_LOSES,
	    LAST_WINS);
	res = prompt_number(mode, prompt, LAST_LOSES, LAST_WINS);
	if (res == RESULT_OK) {
		clear_rows(7);
		print_game_mode(*mode);
		ft_printf("\n");
	}
	return res;
}

Result prompt_picks(t_board *board, unsigned int *picks)
{
	Result res = RESULT_OK;
	char prompt[100];
	const t_row *cur_row = board->rows[board->cur_row];
	const unsigned int max_picks = MIN(MAX_PICKS, cur_row->cur_amount);

	ft_snprintf(
	    prompt, sizeof(prompt), "Pick up to %u%s:\n", max_picks, PRT_PIECE);
	res = prompt_number(
	    picks, prompt, MIN_PICKS, MIN(max_picks, cur_row->cur_amount));
	return res;
}

static Result prompt_number(unsigned int *number,
                            const char *prompt,
                            unsigned int min,
                            unsigned int max)
{
	Result res = RESULT_OK;
	char *line = NULL;
	bool toggle = false;
	const unsigned int prompt_height = ft_strcount_str(prompt, "\n");

	while (true) {
		ft_printf("%s", prompt);
		res = get_input(&line, g_stdin);
		if (res != RESULT_OK) {
			break;
		}
		if (is_valid_number(line, min, max)) {
			*number = ft_atoi(line);
			break;
		}
		free(line);
		clear_rows(prompt_height);
		ft_printf(!toggle ? "Invalid input. " : "Still invalid input. ");
		toggle ^= true;
	}
	free(line);
	return res;
}

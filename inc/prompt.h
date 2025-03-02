#pragma once

#include "alcu.h"

/* prompt */
Result prompt_game_mode(Mode *mode);
Result prompt_picks(t_board *board, unsigned int *picks);

/* prompt_utils */
bool is_valid_number(const char *num, unsigned int lower, unsigned int upper);
Result get_input(char **line, int fd);

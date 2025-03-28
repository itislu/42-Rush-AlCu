#pragma once

#include "alcu.h"

/* prompt */
Result prompt_game_mode(Mode *mode);
Result prompt_picks(t_board *board, unsigned int *picks);

/* prompt_utils */
Result read_line_user(char **line, int fd);
Result read_line_file(char **line, int fd);
bool is_valid_number(const char *num, unsigned int lower, unsigned int upper);

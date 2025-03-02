#pragma once

#include "alcu.h"

/* init */
Result init_board(t_board *board, const char *filename);
void free_board(t_board *board);

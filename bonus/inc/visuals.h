#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <locale.h> // Include this for setlocale
#include "alcu.h"

typedef struct s_size{
	int	x;
	int	y;
} t_size;

typedef struct s_win {
	WINDOW	*win;
	t_size	size;
	t_size	pos;
	int		scroll_offset;
}	t_win;

typedef struct s_ncurses {
	t_win	board;
	t_win	history;
	t_win	input;
	t_size	term;
}	t_ncurses;

void	setup_ncurses(t_ncurses *env);
void	cleanup_ncruses(t_ncurses *env);
Result	run_visuals(t_board *board);

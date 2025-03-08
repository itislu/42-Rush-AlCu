#include "alcu.h"
#include <ncurses.h>

#define MIN_TERMINAL_HEIGTH_FOR_HISTORY 20
#define MIN_TERMINAL_HEIGTH             10
#define MIN_TERMINAL_WIDTH              30

#define NO_GAME_MODE "Game mode is not selected yet"
#define SELECT_GAME_MODE "Select game mode"
#define LAST_TO_PICK_WINS "Last to pick wins"
#define LAST_TO_PICK_LOSES "Last to pick loses"
#define SELECT_PICKS "Select how many picks to remove"
#define WIN "Congrats, you won!"
#define LOSE "You lost..."

#define ESCAPE 27
typedef struct s_size {
	unsigned int x;
	unsigned int y;
} t_size;

typedef struct s_win {
	WINDOW *win;
	t_size size;
	t_size pos;
	unsigned int scroll_offset;
	unsigned int title_height;
} t_win;

typedef struct s_ncurses {
	t_win board;
	t_win history;
	t_win input;
	t_size term;
	bool is_history;
} t_ncurses;

//window updates
void	update_board(t_win *n_board, t_board *board);
void	update_history(t_win *history, t_board *board);
void	update_input(t_win *input, t_board *board);
void	print_res(t_win *input, t_board *board);

//game
Result	game_loop(t_board *board, t_ncurses *env);
void	turn(t_board *board, t_ncurses *env, Player *player);
bool	is_game_end(t_board *board);

//utils
WINDOW	*detect_window(t_ncurses *env, unsigned int y, unsigned int x);
size_t	capped_sub(size_t a, size_t b);
void print_title(t_win *win, char *title);

//ncurses
Result	setup_ncurses(t_ncurses *env);
void	cleanup_ncruses(t_ncurses *env);

//init
Result init_bonus(t_board *board, t_ncurses *env, char *filename);
Result	game_mode_selection(t_ncurses *env, t_board *board);

//event handlers
bool	scroll_handler(t_board *board, t_ncurses *env, int ch);
bool	mouse(t_ncurses *env, t_board *board);

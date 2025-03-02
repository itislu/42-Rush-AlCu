#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define LINE_COUNT 50

int NUM_OPTIONS = 3;

void update_map(WINDOW *win, int scroll_offset) {
	int offset;
    werase(win);
    box(win, 0, 0);
    for (int i = 0; i < LINE_COUNT; i++) {
		offset = i + 1 - scroll_offset;
		if (offset != 0 && offset != (getmaxy(win) - 1))
        	mvwprintw(win, offset, 2, "Line %d: |||||||||||||||||||||||||||||||||||||||||||||||||||||||", i + 1);
    }
    wrefresh(win);
}

void draw_menu(WINDOW *win, int highlight) {
    const char *options[3] = {
        "1 pick",
        "2 picks",
        "3 picks"
    };

    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "Select how many picks you want to remove:");

    for (int i = 0; i < NUM_OPTIONS; i++) {
        if (i == highlight) {
            wattron(win, A_REVERSE);
        }
        mvwprintw(win, 3 + i, 2, "%s", options[i]);
        wattroff(win, A_REVERSE);
    }
    wrefresh(win);
}

int term_height = 0;
int term_width = 0;

int map_x = 0;
int map_y = 0;
int map_pos_x = 0;
int map_pos_y = 0;

int message_x = 0;
int message_y = 0;
int message_pos_x = 0;
int message_pos_y = 0;

int input_x = 0;
int input_y = 0;
int input_pos_x = 0;
int input_pos_y = 0;



int main(void) {
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	mousemask(ALL_MOUSE_EVENTS, NULL);
	srand(time(NULL));

	getmaxyx(stdscr, term_height, term_width);

	map_x = term_width * 2 / 3 - 1;
	map_y = term_height - 1;
	message_x = term_width / 3;
	message_y = term_height * 2 / 3 - 1;
	input_x = term_width / 3;
	input_y = term_height / 3;

	map_pos_x = 1;
	map_pos_y = 1;
	message_pos_x = term_width * 2 / 3;
	message_pos_y = 1;
	input_pos_x = term_width * 2 / 3;
	input_pos_y = term_height * 2 / 3;

	int highlight = 0;
	int map_scroll_offset = 0;

	WINDOW *map_win = newwin(map_y, map_x, map_pos_y, map_pos_x);
	WINDOW *message_win = newwin(message_y, message_x, message_pos_y, message_pos_x);
	WINDOW *input_win = newwin(input_y, input_x, input_pos_y, input_pos_x);
	keypad(input_win, TRUE);

	box(map_win, 0, 0);
	box(input_win, 0, 0);
	box(message_win, 0, 0);

	update_map(map_win, map_scroll_offset);
	draw_menu(input_win, 0);
	wrefresh(message_win);

	while (true)
	{
		draw_menu(input_win, highlight);
		while (true)
		{

			int ch = wgetch(input_win);
			if (ch == '\n')
				break;
			else if (ch == 'q') 
				goto end;
			else if (ch == KEY_UP) {
				highlight = (highlight - 1 + NUM_OPTIONS) % NUM_OPTIONS;
			} else if (ch == KEY_DOWN) {
				highlight = (highlight + 1) % NUM_OPTIONS;
			} else if (ch == KEY_MOUSE) {
				MEVENT event;
				if (getmouse(&event) == OK) {
					if (event.bstate & BUTTON4_PRESSED) {
						if (map_scroll_offset > 0)
						{
							map_scroll_offset--;
							update_map(map_win, map_scroll_offset);
						}
					} else if (event.bstate & BUTTON5_PRESSED) {
						if (map_scroll_offset < - term_height + LINE_COUNT +3)
						{
							map_scroll_offset++;
							update_map(map_win, map_scroll_offset);
						}
		
					}
				}
			}
			draw_menu(input_win, highlight);
		}
		NUM_OPTIONS = rand() % 3 + 1;
		highlight = 0;
		werase(input_win);
		box(input_win, 0, 0);
		mvwprintw(input_win, 1, 1, "Pick again:");
		wrefresh(input_win);
	}

	end:
	delwin(map_win);
	delwin(input_win);
	delwin(message_win);
	endwin();
	return 0;
}

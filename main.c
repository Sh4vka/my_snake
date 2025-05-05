#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

typedef struct {
        int x, y;
} Point;

Point snake[100];
int lenght = 2;
int dir = KEY_RIGHT;
Point food;
Point field[2] = { {50, 5}, {150, 30} }; 

void drow_field() {
        for (int i = field[0].x; i < field[1].x; i++) {
                mvprintw(field[0].y, i, "-");
                mvprintw(field[1].y, i, "-");
        }
        for (int i = field[0].y; i < field[1].y; i++) {
                mvprintw(i, field[0].x, "|");
                mvprintw(i, field[1].x, "|");
        }
}

void init_game() {
        for (int i = 0; i < lenght; i++) {
                snake[i].x = 53;
                snake[i].y = 25;
        }
}

void draw_game() {
        clear();
        drow_field();
        for (int i = 0; i < lenght; i++) {
                mvprintw(snake[i].y, snake[i].x, "O");
        }
        refresh();
}

void update() {
        for (int i = lenght - 1; i > 0; i--) {
                snake[i] = snake[i - 1];
        }
        switch (dir) {
                case KEY_UP:
                        snake[0].y--;
                        break;
                case KEY_DOWN:
                        snake[0].y++;
                        break;
                case KEY_RIGHT:
                        snake[0].x++;
                        break;
                case KEY_LEFT:
                        snake[0].x--;
                        break;
        }
}

int main() {
        initscr();
        curs_set(0);
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        noecho();

        init_game();
        while(1) {
                int ch = getch();
                if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_RIGHT || ch == KEY_LEFT)
                        dir = ch;
                update();
                draw_game();
                usleep(200000);
        }
        endwin();
        return 0;
}

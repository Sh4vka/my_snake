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
Point field[2] = { {50, 3}, {150, 40} }; 

void drow_field() {
        for (int i = field[0].x; i <= field[1].x; i++) {
                mvprintw(field[0].y, i, "-");
                mvprintw(field[1].y, i, "-");
        }
        for (int i = field[0].y; i <= field[1].y; i++) {
                mvprintw(i, field[0].x, "|");
                mvprintw(i, field[1].x, "|");
        }
}

void init_game() {
        for (int i = 0; i < lenght; i++) {
                snake[i].x = 53;
                snake[i].y = 20;
        }
        int width = field[1].x - field[0].x - 1;
        int height = field[1].y - field[0].y - 1;

        food.x = field[0].x + 1 + rand() % width;
        food.y = field[0].y + 1 + rand() % height;
}

void draw_game() {
        clear();
        drow_field();
        for (int i = 0; i < lenght; i++) {
                mvprintw(snake[i].y, snake[i].x, "O");
        }
        mvprintw(food.y, food.x, "@");
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

        if (snake[0].x == food.x && snake[0].y == food.y) {
                lenght++;
                int width = field[1].x - field[0].x - 1;
                int height = field[1].y - field[0].y - 1;

                food.x = field[0].x + 1 + rand() % width;
                food.y = field[0].y + 1 + rand() % height;
        }

        if (snake[0].x == field[0].x 
                || snake[0].x == field[1].x
                || snake[0].y == field[0].y
                || snake[0].y == field[1].y) {
                endwin();
                printf("GAME OVER!\n");
                exit(0);
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
                int old_ch = dir;
                int ch = getch();
                if ((ch == KEY_UP && old_ch != KEY_DOWN) 
                        || (ch == KEY_DOWN && old_ch != KEY_UP) 
                        || (ch == KEY_RIGHT && old_ch != KEY_LEFT)
                        || (ch == KEY_LEFT && old_ch != KEY_RIGHT))
                        dir = ch;
                update();
                draw_game();
                usleep(200000);
        }
        endwin();
        return 0;
}

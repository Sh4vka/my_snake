#define _XOPEN_SOURCE 700

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "config.h"

#define WIGHT  (field[1].x - field[0].x - 1)
#define HEIGHT (field[1].y - field[0].y - 1)

struct timespec ts = {0, T_SLEEP};

typedef struct {
        int x, y;
} Point;

Point snake[100];
int lenght = LENGHT;
int dir = start_dir;
Point food[NUM_FOOD];
Point field[2] = {
        { field_x1, field_y1 },
        { field_x2, field_y2 }
};

void draw_field();
void draw_snake();
void draw_food(int num);
void init_game();
void graw_game();
void  collisian_food();
void collisian_wall();
void update_screen();

void draw_field() {
        for (int i = field[0].x; i <= field[1].x; i++) {
                mvprintw(field[0].y, i, "-");
                mvprintw(field[1].y, i, "-");
        }
        for (int i = field[0].y; i <= field[1].y; i++) {
                mvprintw(i, field[0].x, "|");
                mvprintw(i, field[1].x, "|");
        }
}

void draw_snake() {
        mvprintw(snake[0].y, snake[0].x, "@");
        for (int i = 1; i < lenght; i++)
                mvprintw(snake[i].y, snake[i].x, "O");
}

void draw_food(int num) {
        mvprintw(food[num].y, food[num].x, "&");
}

void init_game() {
        for (int i = 0; i < lenght; i++) {
                snake[i].x = snake_x;
                snake[i].y = snake_y;
        }
        
        for (int i = 0; i < NUM_FOOD; i++){
               food[i].x = field[0].x + 1 + rand() % WIGHT;         
               food[i].y = field[0].y + 1 + rand() % HEIGHT;
        }
}

void draw_game() {
        clear();
        draw_field();
        draw_snake();
        for (int i = 0; i < NUM_FOOD; i++)
                draw_food(i);
        refresh();
}

void collisian_food() {
        for (int i = 0; i < NUM_FOOD; i++){
                if (snake[0].x == food[i].x 
                 && snake[0].y == food[i].y) {
                        lenght++;
                        food[i].x = field[0].x + 1 + rand() % WIGHT;
                        food[i].y = field[0].y + 1 + rand() % HEIGHT;
                        mvprintw(food[i].y, food[i].x, "&");
                }
        }
}

void collisian_wall(){
        if (snake[0].x == field[0].x
         || snake[0].x == field[1].x
         || snake[0].y == field[0].y
         || snake[0].y == field[1].y) {
                endwin();
                printf("GAME OVER!\n");
                exit(0);
        }
}

void update_screen() {
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
        collisian_food();
        collisian_wall();
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
                update_screen();
                draw_game();
                nanosleep(&ts, NULL);
        }
        endwin();

        return 0;
}

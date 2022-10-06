// Copyright [2022] <Team_3>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#define HEIGHT 25
#define WIDTH 80
int start();
void input(int first_matrix[][WIDTH], int second_matrix[][WIDTH]);
void output(int first_matrix[][WIDTH]);
void analize_field(int first_matrix[][WIDTH], int second_matrix[][WIDTH]);
void analize_life(int first_matrix[][WIDTH], int second_matrix[][WIDTH],
                  int height, int width);
void limit_up_down_left_right(int *up, int *down, int *left, int *right);
void die_life_logic(int first_matrix[][WIDTH], int second_matrix[][WIDTH],
                    int height, int width, int sum8);
void next(int first_matrix[][WIDTH], int second_matrix[][WIDTH]);
void speed_change(int *speed, int *exit);

int main() {
  start();
  return 0;
}

int start() {
  int speed = 100;
  int first_matrix[HEIGHT][WIDTH];
  int second_matrix[HEIGHT][WIDTH];
  int exit = 0;
  input(first_matrix, second_matrix);
  initscr();
  noecho();
  raw();  // отключает специальные клавиши
  nodelay(stdscr, 1);
  output(first_matrix);
  while (exit != 1) {
    timeout(speed);
    analize_field(first_matrix, second_matrix);
    output(second_matrix);
    next(first_matrix, second_matrix);
    refresh();
    speed_change(&speed, &exit);
    erase();
  }
  endwin();
  return 0;
}

void input(int first_matrix[][WIDTH], int second_matrix[][WIDTH]) {
  int flag = 0;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      scanf("%1d", &first_matrix[i][j]);
    }
  }
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      second_matrix[i][j] = 0;
    }
  }
  if (freopen("/dev/tty", "r", stdin) == NULL) {
    flag++;
  }
}

void output(int matrix[][WIDTH]) {
  for (int i = 0; i < HEIGHT; i++) {  // выводим на экран
    for (int j = 0; j < WIDTH; j++) {
      if (matrix[i][j] == 1) {
        printw("*");
      } else {
        printw(" ");
      }
    }
    printw("\n");
  }
}

void analize_field(int first_matrix[][WIDTH], int second_matrix[][WIDTH]) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {  // проходим по полю
      analize_life(first_matrix, second_matrix, i, j);
    }
  }
}

void analize_life(int first_matrix[][WIDTH], int second_matrix[][WIDTH],
                  int height, int width) {
  int up = height - 1;  // узнаем положение сверху-снизу, справа-слева
  int down = height + 1;
  int left = width - 1;
  int right = width + 1;
  limit_up_down_left_right(&up, &down, &left, &right);
  int sum8 = first_matrix[up][left] + first_matrix[up][width] +
             first_matrix[up][right] + first_matrix[height][right] +
             first_matrix[down][right] + first_matrix[down][width] +
             first_matrix[down][left] + first_matrix[height][left];
  die_life_logic(first_matrix, second_matrix, height, width, sum8);
}

void limit_up_down_left_right(int *up, int *down, int *left, int *right) {
  if (*up < 0) {  // задаем пределы, зацикленность
    *up = HEIGHT - 1;
  }
  if (*down >= HEIGHT) {
    *down = 0;
  }
  if (*left < 0) {
    *left = WIDTH - 1;
  }
  if (*right >= WIDTH) {
    *right = 0;
  }
}

void die_life_logic(int first_matrix[][WIDTH], int second_matrix[][WIDTH],
                    int height, int width, int sum8) {
  if (first_matrix[height][width] == 0) {  // основная логика игры
    if (sum8 == 3)
      second_matrix[height][width] = 1;
    else
      second_matrix[height][width] = second_matrix[height][width];
  } else if (first_matrix[height][width] == 1) {
    if (sum8 < 2 || sum8 > 3)
      second_matrix[height][width] = 0;
    else
      second_matrix[height][width] = first_matrix[height][width];
  }
}

void next(int first_matrix[][WIDTH], int second_matrix[][WIDTH]) {
  for (int i = 0; i < HEIGHT;
       i++)  // присваиваем старая матрица = новая матрица
    for (int j = 0; j < WIDTH; j++) first_matrix[i][j] = second_matrix[i][j];
}

void speed_change(int *speed, int *exit) {
  char change = ' ';
  change = getch();
  if (((change == 'A') || (change == 'a')) && (*speed < 310)) {
    *speed = *speed + 20;
  }
  if (((change == 'Z') || (change == 'z')) && (*speed > 20)) {
    *speed = *speed - 20;
  }
  if ((change == 'Q') || (change == 'q')) {
    *exit = 1;
  }
}

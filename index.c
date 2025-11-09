#include <stdio.h>
#include <ncurses.h>

#define SIZE 3

void draw_board(int cursor_y, int cursor_x, char board[SIZE][SIZE]) {
    clear();
    mvprintw(0, 0, "Joc X si 0 (foloseste sagetile, SPACE pentru a plasa)");
    mvprintw(1, 0, "--------------------------------");

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int y = 3 + i * 2;
            int x = 4 + j * 4;
            if (i == cursor_y && j == cursor_x)
                attron(A_REVERSE);  // evidențiază poziția curentă

            mvprintw(y, x, " %c ", board[i][j]);

            if (i == cursor_y && j == cursor_x)
                attroff(A_REVERSE);
        }
        printw("\n");
    }
    refresh();
}

int main() {
    initscr();          // pornește ncurses
    noecho();           // nu afișa tastele apăsate
    cbreak();           // dezactivează buffering-ul tastaturii
    keypad(stdscr, TRUE); // activează tastele săgeți

    char board[SIZE][SIZE] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    int cursor_y = 0, cursor_x = 0;
    char turn = 'X';
    int ch;

    draw_board(cursor_y, cursor_x, board);

    while ((ch = getch()) != 'q') {  // 'q' pentru a ieși
        switch (ch) {
            case KEY_UP:
                if (cursor_y > 0) cursor_y--;
                break;
            case KEY_DOWN:
                if (cursor_y < SIZE - 1) cursor_y++;
                break;
            case KEY_LEFT:
                if (cursor_x > 0) cursor_x--;
                break;
            case KEY_RIGHT:
                if (cursor_x < SIZE - 1) cursor_x++;
                break;
            case ' ': // plasare simbol
                if (board[cursor_y][cursor_x] == ' ') {
                    board[cursor_y][cursor_x] = turn;
                    turn = (turn == 'X') ? 'O' : 'X';
                }
                break;
        }
        draw_board(cursor_y, cursor_x, board);
    }

    endwin();  // revine la modul normal de terminal
    return 0;
}

#include <stdio.h>
#include <ncurses.h>

#define SIZE 3

void draw_board(int cursor_y, int cursor_x, char board[SIZE][SIZE]) {
    clear();
    mvprintw(0, 0, "Joc X si 0 (foloseste sagetile, SPACE pentru a plasa)");
    mvprintw(1, 0, "--------------------------------");

    for (int i = 0; i < SIZE; i++) {#include <ncurses.h>
#include <string.h>

#define SIZE 3

// culori
#define COLOR_PLAYER 1
#define COLOR_OPPONENT 2
#define COLOR_HIGHLIGHT 3
#define COLOR_CURSOR 4

// ==================== Funcții GUI ====================
void gui_init() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0); // ascunde cursorul
    if (has_colors()) {
        start_color();
        init_pair(COLOR_PLAYER, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_OPPONENT, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_HIGHLIGHT, COLOR_BLACK, COLOR_YELLOW);
        init_pair(COLOR_CURSOR, COLOR_MAGENTA, COLOR_BLACK);
    }
}

void gui_close() {
    endwin();
}

void draw_message(const char *msg) {
    clear();
    mvprintw(LINES / 2, (COLS - strlen(msg)) / 2, "%s", msg);
    refresh();
}

void draw_waiting_screen() {
    draw_message("Conectare la server...\nAstept un alt jucator sa se alature...");
}

void draw_end_screen(const char *msg) {
    clear();
    mvprintw(LINES / 2 - 1, (COLS - strlen(msg)) / 2, "%s", msg);
    mvprintw(LINES / 2 + 1, (COLS - 25) / 2, "Apasa ENTER pentru un nou joc");
    mvprintw(LINES / 2 + 2, (COLS - 20) / 2, "Apasa Q pentru a iesi");
    refresh();
}

void draw_board(char board[SIZE][SIZE], int cursor_y, int cursor_x, char player_symbol, char opponent_symbol) {
    clear();
    mvprintw(0, 0, "Joc X si 0 (sagetile: muta, SPACE: selecteaza, Q: iesire)");
    mvprintw(1, 0, "--------------------------------------");
    
    // Afiseaza pozitia cursorului in magenta
    attron(COLOR_PAIR(COLOR_CURSOR));
    mvprintw(2, 0, "Pozitie curenta: [%d,%d]", cursor_y, cursor_x);
    attroff(COLOR_PAIR(COLOR_CURSOR));

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int y = 4 + i * 2;
            int x = 4 + j * 4;

            // Culoare X/O
            if (board[i][j] == player_symbol)
                attron(COLOR_PAIR(COLOR_PLAYER));
            else if (board[i][j] == opponent_symbol)
                attron(COLOR_PAIR(COLOR_OPPONENT));

            // Highlight cursor cu magenta
            if (i == cursor_y && j == cursor_x)
                attron(A_REVERSE | COLOR_PAIR(COLOR_CURSOR));

            mvprintw(y, x, " %c ", board[i][j]);

            // Dezactivează highlight și culori separat
            if (i == cursor_y && j == cursor_x)
                attroff(A_REVERSE | COLOR_PAIR(COLOR_CURSOR));
            if (board[i][j] == player_symbol)
                attroff(COLOR_PAIR(COLOR_PLAYER));
            else if (board[i][j] == opponent_symbol)
                attroff(COLOR_PAIR(COLOR_OPPONENT));
        }
    }
    refresh();
}

// ==================== Main ====================
int main() {
    gui_init();

    draw_waiting_screen();
    getch(); // simulează așteptarea serverului

    char board[SIZE][SIZE] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    int cursor_y = 0, cursor_x = 0;
    char player_symbol = 'X';
    char opponent_symbol = 'O';
    char turn = 'X'; // alternare simbol local
    int ch;
    char *end_msg = NULL;

    draw_board(board, cursor_y, cursor_x, player_symbol, opponent_symbol);

    while ((ch = getch()) != 'q') {
        switch (ch) {
            case KEY_UP:    if (cursor_y > 0) cursor_y--; break;
            case KEY_DOWN:  if (cursor_y < SIZE-1) cursor_y++; break;
            case KEY_LEFT:  if (cursor_x > 0) cursor_x--; break;
            case KEY_RIGHT: if (cursor_x < SIZE-1) cursor_x++; break;
            case ' ':
                if (board[cursor_y][cursor_x] == ' ') {
                    board[cursor_y][cursor_x] = turn;
                    turn = (turn == 'X') ? 'O' : 'X';
                }
                break;
            case '\n':
                end_msg = "Ai castigat! 🎉"; // test local
                draw_end_screen(end_msg);
                ch = getch();
                if (ch == 'q') goto exit_program;
                // resetează tabla după ENTER
                for (int i = 0; i < SIZE; i++)
                    for (int j = 0; j < SIZE; j++)
                        board[i][j] = ' ';
                turn = 'X';
                cursor_y = cursor_x = 0;
                end_msg = NULL;
                break;
        }
        if (!end_msg)
            draw_board(board, cursor_y, cursor_x, player_symbol, opponent_symbol);
    }

exit_program:
    gui_close();
    return 0;
}
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

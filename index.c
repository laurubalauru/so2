#include <ncurses.h>
#include <string.h>

/*
 * Joc X si 0 (Tic-Tac-Toe) cu interfață grafică în terminal
 * Foloseste biblioteca ncurses pentru afișare și managementul input-ului
 */

#define SIZE 3  // Dimensiunea tablei de joc (3x3)

// Definirea codurilor de culori
#define COLOR_PLAYER 1      // Verde pentru jucătorul curent
#define COLOR_OPPONENT 2    // Roșu pentru oponent
#define COLOR_HIGHLIGHT 3   // Galben pentru evidențiere
#define COLOR_CURSOR 4      // Magenta pentru cursor
#define COLOR_BORDER 5      // Cyan pentru borduri

// ==================== Funcții GUI ====================

/**
 * Inițializează sistemul ncurses și culorile
 */
void gui_init() {
    initscr();              // Inițializează ncurses
    noecho();               // Ascunde caracterele introduse
    cbreak();               // Dezactivează buffering-ul liniei
    keypad(stdscr, TRUE);   // Activează tastele speciale (săgeți, etc.)
    curs_set(0);            // Ascunde cursorul terminalului
    
    // Verifică dacă terminalul suportă culori
    if (has_colors()) {
        start_color();      // Activează sistemul de culori
        
        // Definirea perechilor de culori (text/fundal)
        init_pair(COLOR_PLAYER, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_OPPONENT, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_HIGHLIGHT, COLOR_BLACK, COLOR_YELLOW);
        init_pair(COLOR_CURSOR, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(COLOR_BORDER, COLOR_CYAN, COLOR_BLACK);
    }
}

/**
 * Închide sistemul ncurses și restaurează terminalul
 */
void gui_close() {
    endwin();
}

/**
 * Afișează un mesaj centrat pe ecran
 * @param msg Mesajul de afișat
 */
void draw_message(const char *msg) {
    clear();  // Șterge ecranul
    // Afișează mesajul centrat pe verticală și orizontală
    mvprintw(LINES / 2, (COLS - strlen(msg)) / 2, "%s", msg);
    refresh();  // Actualizează ecranul
}

/**
 * Afișează ecranul de așteptare pentru conexiune
 */
void draw_waiting_screen() {
    draw_message("Conectare la server...\nAstept un alt jucator sa se alature...");
}

/**
 * Afișează ecranul final cu rezultatul jocului
 * @param msg Mesajul de rezultat (câștig/înfrângere/egal)
 */
void draw_end_screen(const char *msg) {
    clear();
    // Afișează mesajul principal
    mvprintw(LINES / 2 - 1, (COLS - strlen(msg)) / 2, "%s", msg);
    // Afișează instrucțiunile pentru continuare
    mvprintw(LINES / 2 + 1, (COLS - 25) / 2, "Apasa ENTER pentru un nou joc");
    mvprintw(LINES / 2 + 2, (COLS - 20) / 2, "Apasa Q pentru a iesi");
    refresh();
}

/**
 * Afișează scorul jucătorilor
 * @param player_score Scorul jucătorului curent
 * @param opponent_score Scorul oponentului
 * @param player_name Numele jucătorului curent
 * @param opponent_name Numele oponentului
 */
void draw_score(int player_score, int opponent_score, const char *player_name, const char *opponent_name) {
    attron(COLOR_PAIR(COLOR_BORDER));  // Activează culoarea pentru borduri
    mvprintw(2, COLS - 40, "=== SCOR ===");
    mvprintw(3, COLS - 40, "%s: %d", player_name, player_score);
    mvprintw(4, COLS - 40, "%s: %d", opponent_name, opponent_score);
    attroff(COLOR_PAIR(COLOR_BORDER)); // Dezactivează culoarea
}

/**
 * Afișează cine este la mutare
 * @param current_player Numele jucătorului curent
 * @param symbol Simbolul jucătorului (X sau O)
 */
void draw_current_player(const char *current_player, char symbol) {
    attron(COLOR_PAIR(COLOR_BORDER));
    mvprintw(2, 2, "La mutare: %s (%c)", current_player, symbol);
    attroff(COLOR_PAIR(COLOR_BORDER));
}

/**
 * Desenează tabla de joc completă cu borduri și informații
 * @param board Matricea 3x3 cu starea tablei
 * @param cursor_y Poziția Y a cursorului
 * @param cursor_x Poziția X a cursorului
 * @param player_symbol Simbolul jucătorului curent
 * @param opponent_symbol Simbolul oponentului
 */
void draw_board_with_borders(char board[SIZE][SIZE], int cursor_y, int cursor_x, char player_symbol, char opponent_symbol) {
    clear();  // Șterge ecranul pentru redesenare
    
    // Afișează instrucțiunile de control în partea de sus
    mvprintw(0, 0, "Joc X si 0 (sagetile: muta, SPACE: selecteaza, Q: iesire)");
    
    // Desenează linie separatoare sub instrucțiuni
    attron(COLOR_PAIR(COLOR_BORDER));
    mvhline(1, 0, '-', COLS);  // Linie orizontală pe toată lățimea ecranului
    attroff(COLOR_PAIR(COLOR_BORDER));
    
    // Afișează poziția curentă a cursorului
    attron(COLOR_PAIR(COLOR_CURSOR));
    mvprintw(2, 2, "Pozitie curenta: [%d,%d]", cursor_y, cursor_x);
    attroff(COLOR_PAIR(COLOR_CURSOR));

    // Poziția de start pentru tabla de joc (centrată aproximativ)
    int start_y = 6;
    int start_x = 10;
    
    // Desenează tabla cu borduri și conținut
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int y = start_y + i * 2;  // Calcul Y cu spațiu între linii
            int x = start_x + j * 4;  // Calcul X cu spațiu între coloane
            
            // Desenează liniile orizontale între celule
            if (i < SIZE - 1) {
                attron(COLOR_PAIR(COLOR_BORDER));
                mvhline(y + 1, x - 1, '-', 5);  // Linie orizontală
                attroff(COLOR_PAIR(COLOR_BORDER));
            }
            
            // Desenează liniile verticale între celule
            if (j < SIZE - 1) {
                attron(COLOR_PAIR(COLOR_BORDER));
                mvvline(y - 1, x + 2, '|', 3);  // Linie verticală
                attroff(COLOR_PAIR(COLOR_BORDER));
            }
            
            // Desenează colțurile tablei pentru aspect estetic
            if (i == 0 && j == 0) {
                attron(COLOR_PAIR(COLOR_BORDER));
                mvaddch(y - 1, x - 1, '+');
                attroff(COLOR_PAIR(COLOR_BORDER));
            }
            if (i == 0 && j == SIZE - 1) {
                attron(COLOR_PAIR(COLOR_BORDER));
                mvaddch(y - 1, x + 3, '+');
                attroff(COLOR_PAIR(COLOR_BORDER));
            }
            if (i == SIZE - 1 && j == 0) {
                attron(COLOR_PAIR(COLOR_BORDER));
                mvaddch(y + 1, x - 1, '+');
                attroff(COLOR_PAIR(COLOR_BORDER));
            }
            if (i == SIZE - 1 && j == SIZE - 1) {
                attron(COLOR_PAIR(COLOR_BORDER));
                mvaddch(y + 1, x + 3, '+');
                attroff(COLOR_PAIR(COLOR_BORDER));
            }

            // Aplică culori diferite pentru X și O
            if (board[i][j] == player_symbol)
                attron(COLOR_PAIR(COLOR_PLAYER));
            else if (board[i][j] == opponent_symbol)
                attron(COLOR_PAIR(COLOR_OPPONENT));

            // Evidențiază celula unde se află cursorul
            if (i == cursor_y && j == cursor_x)
                attron(A_REVERSE | COLOR_PAIR(COLOR_CURSOR));

            // Afișează conținutul celulei (X, O sau spațiu)
            mvprintw(y, x, " %c ", board[i][j]);

            // Dezactivează evidențierea și culorile
            if (i == cursor_y && j == cursor_x)
                attroff(A_REVERSE | COLOR_PAIR(COLOR_CURSOR));
            if (board[i][j] == player_symbol)
                attroff(COLOR_PAIR(COLOR_PLAYER));
            else if (board[i][j] == opponent_symbol)
                attroff(COLOR_PAIR(COLOR_OPPONENT));
        }
    }
    
    // Afișează scorul și informații despre jucătorul curent
    draw_score(0, 0, "Jucator", "Oponent");
    draw_current_player("Jucator", player_symbol);
    
    refresh();  // Actualizează ecranul cu noile desene
}

// ==================== Funcția Principală ====================

int main() {
    // Inițializează interfața grafică
    gui_init();

    // Afișează ecranul de așteptare pentru conexiune
    draw_waiting_screen();
    getch(); // Simulează așteptarea pentru conexiunea la server

    // Inițializează tabla de joc goală
    char board[SIZE][SIZE] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    // Variabile pentru gestionarea jocului
    int cursor_y = 0, cursor_x = 0;    // Poziția cursorului pe tablă
    char player_symbol = 'X';          // Simbolul jucătorului curent
    char opponent_symbol = 'O';        // Simbolul oponentului
    char turn = 'X';                   // Cine este la mutare ('X' sau 'O')
    int ch;                            // Caracterul introdus de utilizator
    char *end_msg = NULL;              // Mesaj pentru ecranul final
    
    // Variabile pentru scor (vor fi actualizate din rețea)
    int player_score = 0;
    int opponent_score = 0;
    const char *player_name = "Jucator";
    const char *opponent_name = "Oponent";

    // Desenează tabla inițială
    draw_board_with_borders(board, cursor_y, cursor_x, player_symbol, opponent_symbol);

    // Bucla principală a jocului
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case KEY_UP:    if (cursor_y > 0) cursor_y--; break;           // Mișcare sus
            case KEY_DOWN:  if (cursor_y < SIZE-1) cursor_y++; break;      // Mișcare jos
            case KEY_LEFT:  if (cursor_x > 0) cursor_x--; break;           // Mișcare stânga
            case KEY_RIGHT: if (cursor_x < SIZE-1) cursor_x++; break;      // Mișcare dreapta
            
            case ' ':
                // Mutare: plasează simbolul pe tablă dacă celula este liberă
                if (board[cursor_y][cursor_x] == ' ') {
                    board[cursor_y][cursor_x] = turn;
                    turn = (turn == 'X') ? 'O' : 'X';  // Schimbă jucătorul
                    
                    // Actualizează afișarea jucătorului curent
                    if (turn == player_symbol) {
                        draw_current_player(player_name, player_symbol);
                    } else {
                        draw_current_player(opponent_name, opponent_symbol);
                    }
                }
                break;
                
            case '\n':
                // Simulare: jucătorul câștigă (pentru testare)
                player_score++;
                end_msg = "Ai castigat! 🎉";
                draw_end_screen(end_msg);
                
                // Așteaptă input pentru continuare sau ieșire
                ch = getch();
                if (ch == 'q') {
                    gui_close();
                    return 0;
                }
                
                // Resetează tabla pentru un joc nou
                for (int i = 0; i < SIZE; i++)
                    for (int j = 0; j < SIZE; j++)
                        board[i][j] = ' ';
                turn = 'X';
                cursor_y = cursor_x = 0;
                end_msg = NULL;
                break;
        }
        
        // Redesenează tabla dacă nu suntem pe ecranul final
        if (!end_msg)
            draw_board_with_borders(board, cursor_y, cursor_x, player_symbol, opponent_symbol);
    }

    // Închide interfața grafică și eliberează resursele
    gui_close();
    return 0;
}

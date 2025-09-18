#include <iostream>
#include <vector>
#include <limits>

using namespace std;

constexpr int ROWS = 6;
constexpr int COLS = 7;
constexpr int EMPTY = 0;
constexpr int PLAYER1 = 1; // X
constexpr int PLAYER2 = 2; // O

// --- Prototipos ---
void printBoard(const vector<vector<int>>& board);
int readColumn(const vector<vector<int>>& board);
bool isValidMove(const vector<vector<int>>& board, int col);
int dropPiece(vector<vector<int>>& board, int col, int player);
bool isFull(const vector<vector<int>>& board);
bool checkWin(const vector<vector<int>>& board, int row, int col, int player);
int countDirection(const vector<vector<int>>& board, int r, int c, int dr, int dc, int player);

int main() {
    vector<vector<int>> board(ROWS, vector<int>(COLS, EMPTY));
    int turno = PLAYER1; // empieza jugador 1
    printBoard(board);

    while (true) {
        cout << (turno == PLAYER1 ? "Turno: Jugador 1 (X)\n" : "Turno: Jugador 2 (O)\n");

        int col = readColumn(board);
        int row = dropPiece(board, col, turno);

        // seguridad: si por alguna razon no se pudo colocar
        if (row == -1) {
            cout << "Error al colocar la ficha. Intenta otra vez.\n";
            continue;
        }

        printBoard(board);

        if (checkWin(board, row, col, turno)) {
            cout << (turno == PLAYER1 ? "¡Jugador 1 (X) gana!\n" : "¡Jugador 2 (O) gana!\n");
            break;
        }

        if (isFull(board)) {
            cout << "Empate. ¡Tablero lleno!\n";
            break;
        }

        // cambiar turno
        turno = (turno == PLAYER1) ? PLAYER2 : PLAYER1;
    }

    cout << "Fin del juego.\n";
    return 0;
}

// --- Implementaciones ---
void printBoard(const vector<vector<int>>& board) {
    cout << '\n';
    for (int r = 0; r < ROWS; ++r) {
        cout << "|";
        for (int c = 0; c < COLS; ++c) {
            char ch = ' ';
            if (board[r][c] == PLAYER1) ch = 'X';
            else if (board[r][c] == PLAYER2) ch = 'O';
            cout << " " << ch << " |";
        }
        cout << '\n';
        // separador
        for (int i = 0; i < COLS; ++i) cout << "+---";
        cout << "+\n";
    }
    // números de columna
    cout << " ";
    for (int c = 1; c <= COLS; ++c) {
        cout << " " << c << "  ";
    }
    cout << "\n\n";
}

int readColumn(const vector<vector<int>>& board) {
    int col;
    while (true) {
        cout << "Elige columna (1-" << COLS << "): ";
        if (!(cin >> col)) {
            // limpiar entrada inválida
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Ingresa un número.\n";
            continue;
        }
        col -= 1; // usuario ve 1..COLS
        if (col < 0 || col >= COLS) {
            cout << "Columna fuera de rango.\n";
            continue;
        }
        if (!isValidMove(board, col)) {
            cout << "Columna llena. Elige otra.\n";
            continue;
        }
        return col;
    }
}

bool isValidMove(const vector<vector<int>>& board, int col) {
    return board[0][col] == EMPTY;
}

int dropPiece(vector<vector<int>>& board, int col, int player) {
    for (int r = ROWS - 1; r >= 0; --r) {
        if (board[r][col] == EMPTY) {
            board[r][col] = player;
            return r;
        }
    }
    return -1; // columna llena (debería no ocurrir si isValidMove fue true)
}

bool isFull(const vector<vector<int>>& board) {
    for (int c = 0; c < COLS; ++c) {
        if (board[0][c] == EMPTY) return false;
    }
    return true;
}

bool checkWin(const vector<vector<int>>& board, int row, int col, int player) {
    if (row < 0 || col < 0) return false;
    // horizontal, vertical, diagonal \ y diagonal /
    return (countDirection(board, row, col, 0, 1, player) + countDirection(board, row, col, 0, -1, player) - 1 >= 4) ||
           (countDirection(board, row, col, 1, 0, player) + countDirection(board, row, col, -1, 0, player) - 1 >= 4) ||
           (countDirection(board, row, col, 1, 1, player) + countDirection(board, row, col, -1, -1, player) - 1 >= 4) ||
           (countDirection(board, row, col, 1, -1, player) + countDirection(board, row, col, -1, 1, player) - 1 >= 4);
}

int countDirection(const vector<vector<int>>& board, int r, int c, int dr, int dc, int player) {
    int count = 0;
    int rr = r, cc = c;
    while (rr >= 0 && rr < ROWS && cc >= 0 && cc < COLS && board[rr][cc] == player) {
        ++count;
        rr += dr;
        cc += dc;
    }
    return count;
}

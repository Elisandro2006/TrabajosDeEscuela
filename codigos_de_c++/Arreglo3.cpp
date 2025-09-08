#include <iostream>
using namespace std;

int main() {
    int matriz[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int num_filas = 3;
    int num_columnas = 3;

    for (int j = 0; j < num_columnas; j++) {
        cout << "--- Columna " << j << " ---\n";
        for (int i = 0; i < num_filas; i++) {
            cout << matriz[i][j] << " ";
        }
        cout << "\n\n";
    }
    return 0;
}

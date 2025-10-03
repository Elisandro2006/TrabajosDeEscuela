#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    int matriz[3][3];
    cout << "Ingrese los valores de la matriz 3x3:\n";
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            cin >> matriz[i][j];


    for (int i = 0; i < 3; i++)
        sort(matriz[i], matriz[i] + 3);

    cout << "Matriz con filas ordenadas:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            cout << matriz[i][j] << " ";
        cout << endl;
    }
    return 0;
}

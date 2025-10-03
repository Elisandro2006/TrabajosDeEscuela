#include <iostream>
using namespace std;

int main() {
    int cubo[3][3][3];
    cout << "Ingrese los valores de la matriz 3x3x3:\n";
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                cin >> cubo[i][j][k];

    for (int capa = 0; capa < 3; capa++) {
        int suma = 0;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                suma += cubo[capa][i][j];
        double promedio = suma / 9.0;
        cout << "Promedio de la capa " << capa << " = " << promedio << endl;
    }
    return 0;
}

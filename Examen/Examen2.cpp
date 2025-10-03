#include <iostream>
using namespace std;

int main() {
    int arr[10];
    cout << "Ingrese 10 numeros: ";
    for (int i = 0; i < 10; i++) cin >> arr[i];

    cout << "Arreglo sin duplicados: ";
    for (int i = 0; i < 10; i++) {
        bool duplicado = false;
        for (int j = 0; j < i; j++) {
            if (arr[i] == arr[j]) {
                duplicado = true;
                break;
            }
        }
        if (!duplicado) cout << arr[i] << " ";
    }
    return 0;
}

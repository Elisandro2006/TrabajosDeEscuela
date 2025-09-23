#include <iostream>

int main() {
    int arreglo[5] = {24, 2, 11, 6, 3};
    int i, j, aux;
    std::cout << "Antes de ordenar: " << std::endl;
    for (i = 0; i < 5; i++) {
        std::cout << arreglo[i];
    }
    std::cout << std::endl;
    std::cout << "Despues de ordenar: " << std::endl;
    for (i = 1; i < 5; i++) {
        aux = arreglo[i];
        j = i - 1;
        while (j >= 0 && arreglo[j] > aux) {
            arreglo[j + 1] = arreglo[j];
            j--;
        }
        arreglo[j + 1] = aux;
    }
    for (i = 0; i < 5; i++) {
        std::cout << arreglo[i];
    }
    return 0;
}// Algoritmo de ordenamiento por inserción
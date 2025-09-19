#include <iostream>
#include <cstdlib>  // Para el uso random
#include <ctime>    //para el tiempo

int main(){

    srand(time(0));
    
    int num = 10;
    int array[num];

    for (int i = 0; i < num; i++) { //Llenado aleatorio
        array[i] = 1 + rand() % 100;
    }

    std::cout << "Los numeros del arreglo son: "; //Mostrar array
    for (int i = 0; i < num; i++) {
        std::cout << array[i] << " ";
    }

    for (int i = 0; i < num - 1; i++) { //Ordenamiento mayor a menor
        for (int j = 0; j < num - 1 - i; j++) {
            if (array[j] < array[j + 1]) {
                // Intercambio de numero
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    std::cout << '\n'; //Espacio entre respuestas
    std::cout << "Los numeros de mayor a menor: ";
    for (int i = 0; i < num; i++) {
        std::cout << array[i] << " ";
    }
}
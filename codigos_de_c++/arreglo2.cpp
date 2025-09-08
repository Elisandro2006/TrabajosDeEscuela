#include <iostream>

int main(){
    int arr[] = {5, 3, 8, 1, 2};
    int tamanoOriginal = 5;
    int elementoIns = 4;
    int posicion = 2;
    std::cout << "Arreglo original: ";
    for(int i = 0; i < tamanoOriginal; i++){
        std::cout << arr[i] << " ";
    }
    int nuevoArr[tamanoOriginal + 1];
    for (int i = 0; i < posicion; i++){
        nuevoArr[i] = arr[i];
    }
    nuevoArr[posicion] = elementoIns;
    for (int i = posicion; i < tamanoOriginal; i++){
        nuevoArr[i + 1] = arr[i];
    }
    std::cout << "\nArreglo con elemento insertado: "; 
    for(int i = 0; i <= tamanoOriginal; i++){
        std::cout << nuevoArr[i] << " ";
    }
    std::cout << "\nEliminacion por posicion: ";
    int posEliminar = 3;
    for (int i = 0; i < tamanoOriginal + 1; i++){
        if (i < posEliminar){
            std::cout << nuevoArr[i] << " ";
        } else if (i > posEliminar){
            std::cout << nuevoArr[i] << " ";
        }
    }
    std::cout << "\nBusqueda lineal de elemento: ";
    int elemnntoBuscar = 5;
    bool encontrado = false;
    for (int i = 0; i < tamanoOriginal + 1; i++){
        if (nuevoArr[i] == elemnntoBuscar){
            std::cout << "Elemento " << elemnntoBuscar << " encontrado en la posicion " << i << "\n";
            encontrado = true;
            break;
        }
         if (!encontrado){
        std::cout << "Elemento " << elemnntoBuscar << " no encontrado\n";

    } 
    return 0;
}
}

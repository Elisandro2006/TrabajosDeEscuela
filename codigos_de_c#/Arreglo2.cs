using System;

class Program {
    static void Main() {
        int[] arr = {5, 3, 8, 1, 2};
        int tamanoOriginal = arr.Length;
        int elementoIns = 4;
        int posicion = 2;

        Console.Write("Arreglo original: ");
        for (int i = 0; i < tamanoOriginal; i++) {
            Console.Write(arr[i] + " ");
        }

        int[] nuevoArr = new int[tamanoOriginal + 1];
        for (int i = 0; i < posicion; i++) {
            nuevoArr[i] = arr[i];
        }
        nuevoArr[posicion] = elementoIns;
        for (int i = posicion; i < tamanoOriginal; i++) {
            nuevoArr[i + 1] = arr[i];
        }

        Console.Write("\nArreglo con elemento insertado: ");
        for (int i = 0; i < nuevoArr.Length; i++) {
            Console.Write(nuevoArr[i] + " ");
        }

        Console.Write("\nEliminacion por posicion: ");
        int posEliminar = 3;
        for (int i = 0; i < nuevoArr.Length; i++) {
            if (i != posEliminar) {
                Console.Write(nuevoArr[i] + " ");
            }
        }

        Console.Write("\nBusqueda lineal de elemento: ");
        int elementoBuscar = 5;
        bool encontrado = false;
        for (int i = 0; i < nuevoArr.Length; i++) {
            if (nuevoArr[i] == elementoBuscar) {
                Console.WriteLine("Elemento " + elementoBuscar + " encontrado en la posicion " + i);
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            Console.WriteLine("Elemento " + elementoBuscar + " no encontrado");
        }
    }
}

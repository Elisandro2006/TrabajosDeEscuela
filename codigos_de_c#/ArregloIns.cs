using System;

class ArregloIns {
    static void Main() {
        int[] arreglo = {24, 2, 11, 6, 3};
        int i, j, aux;

        Console.WriteLine("Antes de ordenar:");
        for (i = 0; i < arreglo.Length; i++) {
            Console.Write(arreglo[i]);
        }
        Console.WriteLine();

        // Algoritmo de inserción
        for (i = 1; i < arreglo.Length; i++) {
            aux = arreglo[i];
            j = i - 1;
            while (j >= 0 && arreglo[j] > aux) {
                arreglo[j + 1] = arreglo[j];
                j--;
            }
            arreglo[j + 1] = aux;
        }

        Console.WriteLine("Despues de ordenar:");
        for (i = 0; i < arreglo.Length; i++) {
            Console.Write(arreglo[i]);
        }
    }
}


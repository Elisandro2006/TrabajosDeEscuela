using System;

class Program {
    static void Main() {
        int[,] matriz = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        };

        int num_filas = matriz.GetLength(0);
        int num_columnas = matriz.GetLength(1);

        for (int j = 0; j < num_columnas; j++) {
            Console.WriteLine($"--- Columna {j} ---");
            for (int i = 0; i < num_filas; i++) {
                Console.Write(matriz[i, j] + " ");
            }
            Console.WriteLine("\n");
        }
    }
}

public class Arreglo3 {
    public static void main(String[] args) {
        int[][] matriz = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        };

        int num_filas = matriz.length;
        int num_columnas = matriz[0].length;

        for (int j = 0; j < num_columnas; j++) {
            System.out.println("--- Columna " + j + " ---");
            for (int i = 0; i < num_filas; i++) {
                System.out.print(matriz[i][j] + " ");
            }
            System.out.println("\n");
        }
    }
}

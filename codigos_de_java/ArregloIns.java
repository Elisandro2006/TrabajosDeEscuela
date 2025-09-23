public class ArregloIns {
    public static void main(String[] args) {
        int[] arreglo = {24, 2, 11, 6, 3};
        int i, j, aux;

        System.out.println("Antes de ordenar:");
        for (i = 0; i < arreglo.length; i++) {
            System.out.print(arreglo[i]);
        }
        System.out.println();

        // Algoritmo de inserción
        for (i = 1; i < arreglo.length; i++) {
            aux = arreglo[i];
            j = i - 1;
            while (j >= 0 && arreglo[j] > aux) {
                arreglo[j + 1] = arreglo[j];
                j--;
            }
            arreglo[j + 1] = aux;
        }

        System.out.println("Despues de ordenar:");
        for (i = 0; i < arreglo.length; i++) {
            System.out.print(arreglo[i]);
        }
    }
}

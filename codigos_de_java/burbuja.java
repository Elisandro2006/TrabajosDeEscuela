import java.util.Random;

public class  burbuja{
    public static void main(String[] args) {
        Random rand = new Random();
        int num = 10;
        int[] array = new int[num];

        // Llenado aleatorio
        for (int i = 0; i < num; i++) {
            array[i] = 1 + rand.nextInt(100); // entre 1 y 100
        }

        System.out.print("Los numeros del arreglo son: ");
        for (int val : array) {
            System.out.print(val + " ");
        }

        // Ordenamiento burbuja (mayor a menor)
        for (int i = 0; i < num - 1; i++) {
            for (int j = 0; j < num - 1 - i; j++) {
                if (array[j] < array[j + 1]) {
                    int temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }

        System.out.print("\nLos numeros de mayor a menor: ");
        for (int val : array) {
            System.out.print(val + " ");
        }
    }
}

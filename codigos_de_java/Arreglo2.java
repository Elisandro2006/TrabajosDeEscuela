public class Arreglo2 {
    public static void main(String[] args) {
        int[] arr = {5, 3, 8, 1, 2};
        int tamanoOriginal = arr.length;
        int elementoIns = 4;
        int posicion = 2;

        System.out.print("Arreglo original: ");
        for (int i = 0; i < tamanoOriginal; i++) {
            System.out.print(arr[i] + " ");
        }

        int[] nuevoArr = new int[tamanoOriginal + 1];
        for (int i = 0; i < posicion; i++) {
            nuevoArr[i] = arr[i];
        }
        nuevoArr[posicion] = elementoIns;
        for (int i = posicion; i < tamanoOriginal; i++) {
            nuevoArr[i + 1] = arr[i];
        }

        System.out.print("\nArreglo con elemento insertado: ");
        for (int i = 0; i < nuevoArr.length; i++) {
            System.out.print(nuevoArr[i] + " ");
        }

        System.out.print("\nEliminacion por posicion: ");
        int posEliminar = 3;
        for (int i = 0; i < nuevoArr.length; i++) {
            if (i != posEliminar) {
                System.out.print(nuevoArr[i] + " ");
            }
        }

        System.out.print("\nBusqueda lineal de elemento: ");
        int elementoBuscar = 5;
        boolean encontrado = false;
        for (int i = 0; i < nuevoArr.length; i++) {
            if (nuevoArr[i] == elementoBuscar) {
                System.out.println("Elemento " + elementoBuscar + " encontrado en la posicion " + i);
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            System.out.println("Elemento " + elementoBuscar + " no encontrado");
        }
    }
}

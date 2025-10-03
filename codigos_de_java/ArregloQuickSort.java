
public class ArregloQuickSort {
    
    static void swap(int[] arr, int i, int j) {
        int t = arr[i];
        arr[i] = arr[j];
        arr[j] = t;
    }

    static int particion(int[] arr, int low, int high) {
        int pivot = arr[high];
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr, i, j);
            }
        }
        swap(arr, i + 1, high);
        return (i + 1);
    }

    static void quicksort(int[] arr, int low, int high) {
        if (low < high) {
            int pi = particion(arr, low, high);
            quicksort(arr, low, pi - 1);
            quicksort(arr, pi + 1, high);
        }
    }

    static void printArray(int[] arr) {
        for (int x : arr) {
            System.out.print(x + " ");
        }
        System.out.println();
    }

    public static void main(String[] args) {
        int[] arr = {10, 20, 3, 60, 41};
        System.out.print("Array Original: ");
        printArray(arr);

        quicksort(arr, 0, arr.length - 1);

        System.out.print("Array Ordenado: ");
        printArray(arr);
    }
}

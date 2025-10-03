#include <iostream>
#include <vector>
#include <algorithm> //para el std::swap

void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}
int particion(std::vector<int>& arr, int low, int high){
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low;j <= high - 1; j++){
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
        
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
void quicksort(std::vector<int>& arr, int low, int high){
    if (low < high){
        int pi = particion(arr, low, high);

        quicksort(arr, low, pi -1);
        quicksort(arr, pi + 1,high);
    }
}
void printArray(const std::vector<int>& arr){
    for (int x: arr){
        std::cout << x << " ";
    }
    std::cout << std::endl;
}
int main(){
    std::vector<int> arr = {10,20,3,60,41};
    std::cout << "Array Original: ";
    printArray(arr);
    
    std::cout << "Array Ordenado: ";
    quicksort(arr, 0, arr.size() -1);
    printArray(arr);
    
    return 0;
}
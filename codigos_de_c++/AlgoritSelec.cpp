#include <iostream>

int main()
{
    int arr[] = {24, 3, 40, 2, 11}; 
    int n = sizeof(arr) / sizeof(arr[0]);
    std::cout << "Array no ordenado: ";
    for (int i = 0; i < n; i++)
    {
        std::cout << arr[i] << " ";
    }
   std::cout << std::endl;

    for (int i = 0; i < n - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }
        if (minIndex != i)
        {
            std::swap(arr[i], arr[minIndex]);
        }
    }
    std::cout << "Array ordenado: ";
    for (int i = 0; i < n; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
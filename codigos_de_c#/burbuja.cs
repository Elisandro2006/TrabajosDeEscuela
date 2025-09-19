using System;

class Program
{
    static void Main()
    {
        Random rand = new Random();
        int num = 10;
        int[] array = new int[num];

        // Llenado aleatorio
        for (int i = 0; i < num; i++)
        {
            array[i] = rand.Next(1, 101); // entre 1 y 100
        }

        Console.Write("Los numeros del arreglo son: ");
        foreach (int val in array)
        {
            Console.Write(val + " ");
        }

        // Ordenamiento burbuja (mayor a menor)
        for (int i = 0; i < num - 1; i++)
        {
            for (int j = 0; j < num - 1 - i; j++)
            {
                if (array[j] < array[j + 1])
                {
                    int temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }

        Console.WriteLine("\nLos numeros de mayor a menor: ");
        foreach (int val in array)
        {
            Console.Write(val + " ");
        }
    }
}

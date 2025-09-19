import random

def main():
    num = 10
    array = []

    # Llenado aleatorio
    for i in range(num):
        array.append(random.randint(1, 100))  # 1 a 100

    print("Los numeros del arreglo son:", *array)

    # Ordenamiento burbuja (mayor a menor)
    for i in range(num - 1):
        for j in range(num - 1 - i):
            if array[j] < array[j + 1]:
                array[j], array[j + 1] = array[j + 1], array[j]

    print("Los numeros de mayor a menor:", *array)

main()

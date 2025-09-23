arreglo = [24, 2, 11, 6, 3]

print("Antes de ordenar:")
print("".join(str(x) for x in arreglo))

# Algoritmo de inserción
for i in range(1, len(arreglo)):
    aux = arreglo[i]
    j = i - 1
    while j >= 0 and arreglo[j] > aux:
        arreglo[j + 1] = arreglo[j]
        j -= 1
    arreglo[j + 1] = aux

print("Despues de ordenar:")
print("".join(str(x) for x in arreglo))

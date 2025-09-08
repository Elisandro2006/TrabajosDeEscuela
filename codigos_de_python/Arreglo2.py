arr = [5, 3, 8, 1, 2]
tamanoOriginal = len(arr)
elementoIns = 4
posicion = 2

print("Arreglo original:", " ".join(map(str, arr)))

# Insertar elemento
nuevoArr = arr[:posicion] + [elementoIns] + arr[posicion:]
print("Arreglo con elemento insertado:", " ".join(map(str, nuevoArr)))

# Eliminar por posición
posEliminar = 3
arrEliminado = [nuevoArr[i] for i in range(len(nuevoArr)) if i != posEliminar]
print("Eliminacion por posicion:", " ".join(map(str, arrEliminado)))

# Búsqueda lineal
elementoBuscar = 5
encontrado = False
for i in range(len(nuevoArr)):
    if nuevoArr[i] == elementoBuscar:
        print(f"Elemento {elementoBuscar} encontrado en la posicion {i}")
        encontrado = True
        break

if not encontrado:
    print(f"Elemento {elementoBuscar} no encontrado")

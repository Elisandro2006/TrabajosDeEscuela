matriz = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
]

num_filas = len(matriz)
num_columnas = len(matriz[0])

for j in range(num_columnas): 
    print(f"--- Columna {j} ---")
    for i in range(num_filas):
        print(matriz[i][j], end=" ") 
    print("\n")
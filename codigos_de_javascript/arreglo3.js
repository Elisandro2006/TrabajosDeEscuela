let matriz = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
];

let num_filas = matriz.length;
let num_columnas = matriz[0].length;

for (let j = 0; j < num_columnas; j++) {
    console.log(`--- Columna ${j} ---`);
    for (let i = 0; i < num_filas; i++) {
        process.stdout.write(matriz[i][j] + " ");
    }
    console.log("\n");
}

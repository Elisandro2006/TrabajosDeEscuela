let arreglo = [24, 2, 11, 6, 3];
let i, j, aux;

console.log("Antes de ordenar:");
console.log(arreglo.join(""));

for (i = 1; i < arreglo.length; i++) {
    aux = arreglo[i];
    j = i - 1;
    while (j >= 0 && arreglo[j] > aux) {
        arreglo[j + 1] = arreglo[j];
        j--;
    }
    arreglo[j + 1] = aux;
}

console.log("Despues de ordenar:");
console.log(arreglo.join(""));

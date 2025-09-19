function main() {
    let num = 10;
    let array = [];

    // Llenado aleatorio
    for (let i = 0; i < num; i++) {
        array[i] = Math.floor(Math.random() * 100) + 1; // 1 a 100
    }

    console.log("Los numeros del arreglo son: " + array.join(" "));

    // Ordenamiento burbuja (mayor a menor)
    for (let i = 0; i < num - 1; i++) {
        for (let j = 0; j < num - 1 - i; j++) {
            if (array[j] < array[j + 1]) {
                let temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    console.log("Los numeros de mayor a menor: " + array.join(" "));
}

main();

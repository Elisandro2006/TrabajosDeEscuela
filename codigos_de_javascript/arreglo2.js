let arr = [5, 3, 8, 1, 2];
let tamanoOriginal = arr.length;
let elementoIns = 4;
let posicion = 2;

console.log("Arreglo original:", arr.join(" "));

let nuevoArr = [];
for (let i = 0; i < posicion; i++) {
    nuevoArr[i] = arr[i];
}
nuevoArr[posicion] = elementoIns;
for (let i = posicion; i < tamanoOriginal; i++) {
    nuevoArr[i + 1] = arr[i];
}

console.log("Arreglo con elemento insertado:", nuevoArr.join(" "));

let posEliminar = 3;
let arrEliminado = [];
for (let i = 0; i < nuevoArr.length; i++) {
    if (i !== posEliminar) {
        arrEliminado.push(nuevoArr[i]);
    }
}
console.log("Eliminacion por posicion:", arrEliminado.join(" "));

let elementoBuscar = 3;
let encontrado = false;
for (let i = 0; i < nuevoArr.length; i++) {
    if (nuevoArr[i] === elementoBuscar) {
        console.log(`Elemento ${elementoBuscar} encontrado en la posicion ${i}`);
        encontrado = true;
        break;
    }
}
if (!encontrado) {
    console.log(`Elemento ${elementoBuscar} no encontrado`);
}

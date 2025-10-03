function swap(arr, i, j) {
    let t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

function particion(arr, low, high) {
    let pivot = arr[high];
    let i = low - 1;

    for (let j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr, i, j);
        }
    }
    swap(arr, i + 1, high);
    return i + 1;
}

function quicksort(arr, low, high) {
    if (low < high) {
        let pi = particion(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

function printArray(arr) {
    console.log(arr.join(" "));
}

let arr = [10, 20, 3, 60, 41];
console.log("Array Original: ");
printArray(arr);

quicksort(arr, 0, arr.length - 1);

console.log("Array Ordenado: ");
printArray(arr);

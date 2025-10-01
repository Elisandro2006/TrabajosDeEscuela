let arr = [24, 3, 40, 2, 11];
let n = arr.length;

console.log("Array no ordenado: " + arr.join(" "));

for (let i = 0; i < n - 1; i++) {
    let minIndex = i;
    for (let j = i + 1; j < n; j++) {
        if (arr[j] < arr[minIndex]) {
            minIndex = j;
        }
    }
    if (minIndex !== i) {
        [arr[i], arr[minIndex]] = [arr[minIndex], arr[i]]; // swap
    }
}

console.log("Array ordenado: " + arr.join(" "));

arr = [24, 3, 40, 2, 11]
n = len(arr)

print("Array no ordenado:", *arr)

for i in range(n - 1):
    min_index = i
    for j in range(i + 1, n):
        if arr[j] < arr[min_index]:
            min_index = j
    if min_index != i:
        arr[i], arr[min_index] = arr[min_index], arr[i]  # swap

print("Array ordenado:", *arr)

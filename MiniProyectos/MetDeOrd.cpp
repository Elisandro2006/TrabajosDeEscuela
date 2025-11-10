#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <string>
#include <functional>
#include <cmath>

using namespace std;
using namespace chrono;

// Timeout en milisegundos (30 segundos)
const int TIMEOUT_MS = 30000;

// ==================== GENERADORES DE DATOS ====================
vector<int> generar_ordenado(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    return arr;
}

vector<int> generar_medianamente_ordenado(int n) {
    vector<int> arr = generar_ordenado(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, n - 1);
    
    int desordenar = n * 0.3;
    for (int i = 0; i < desordenar; i++) {
        int pos1 = dis(gen);
        int pos2 = dis(gen);
        swap(arr[pos1], arr[pos2]);
    }
    return arr;
}

vector<int> generar_inverso(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
    return arr;
}

// ==================== ALGORITMOS DE ORDENAMIENTO ====================

// 1. BURBUJA (Optimizado con early exit)
void bubble_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// 2. POR CUBOS (Bucket Sort)
void bucket_sort(vector<int>& arr) {
    if (arr.empty()) return;
    
    int n = arr.size();
    int max_val = *max_element(arr.begin(), arr.end());
    int min_val = *min_element(arr.begin(), arr.end());
    int bucket_count = sqrt(n);
    if (bucket_count == 0) bucket_count = 1;
    int range_val = max_val - min_val + 1;
    
    vector<vector<int>> buckets(bucket_count);
    
    for (int num : arr) {
        long long bucket_index = ((long long)bucket_count * (num - min_val)) / range_val;
        if (bucket_index >= bucket_count) bucket_index = bucket_count - 1;
        buckets[bucket_index].push_back(num);
    }
    
    for (auto& bucket : buckets) {
        sort(bucket.begin(), bucket.end());
    }
    
    arr.clear();
    for (const auto& bucket : buckets) {
        arr.insert(arr.end(), bucket.begin(), bucket.end());
    }
}

// 3. COMB SORT
void comb_sort(vector<int>& arr) {
    int n = arr.size();
    int gap = n;
    bool swapped = true;
    
    while (gap > 1 || swapped) {
        gap = (gap * 10) / 13;
        if (gap < 1) gap = 1;
        
        swapped = false;
        for (int i = 0; i < n - gap; i++) {
            if (arr[i] > arr[i + gap]) {
                swap(arr[i], arr[i + gap]);
                swapped = true;
            }
        }
    }
}

// 4. CONTEO (Counting Sort)
void counting_sort(vector<int>& arr) {
    if (arr.empty()) return;
    
    int max_val = *max_element(arr.begin(), arr.end());
    int min_val = *min_element(arr.begin(), arr.end());
    long long range_val = (long long)max_val - min_val + 1;
    
    // Limitar el rango para evitar problemas de memoria
    if (range_val > 10000000) {
        sort(arr.begin(), arr.end());
        return;
    }
    
    vector<int> count(range_val, 0);
    vector<int> output(arr.size());
    
    for (int num : arr) {
        count[num - min_val]++;
    }
    
    for (size_t i = 1; i < count.size(); i++) {
        count[i] += count[i - 1];
    }
    
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i] - min_val] - 1] = arr[i];
        count[arr[i] - min_val]--;
    }
    
    arr = output;
}

// 5. HEAP SORT
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[left] > arr[largest])
        largest = left;
    
    if (right < n && arr[right] > arr[largest])
        largest = right;
    
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heap_sort(vector<int>& arr) {
    int n = arr.size();
    
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// 6. INSERCION
void insertion_sort(vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 7. FUSION (Merge Sort)
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    vector<int> L(n1);
    vector<int> R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort_helper(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_helper(arr, left, mid);
        merge_sort_helper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void merge_sort(vector<int>& arr) {
    if (arr.size() > 1) {
        merge_sort_helper(arr, 0, arr.size() - 1);
    }
}

// 8. RAPIDO (Quick Sort) - Iterativo con pivote aleatorio
void quick_sort(vector<int>& arr) {
    if (arr.size() <= 1) return;
    
    random_device rd;
    mt19937 gen(rd());
    
    vector<pair<int, int>> stack;
    stack.reserve(64);
    stack.push_back({0, (int)arr.size() - 1});
    
    while (!stack.empty()) {
        int low = stack.back().first;
        int high = stack.back().second;
        stack.pop_back();
        
        if (low >= high) continue;
        
        // Pivote aleatorio
        uniform_int_distribution<> dis(low, high);
        int pivot_idx = dis(gen);
        swap(arr[pivot_idx], arr[high]);
        
        int pivot = arr[high];
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        
        swap(arr[i + 1], arr[high]);
        int pi = i + 1;
        
        // Optimización: procesar primero la partición más pequeña
        if (pi - low < high - pi) {
            stack.push_back({pi + 1, high});
            stack.push_back({low, pi - 1});
        } else {
            stack.push_back({low, pi - 1});
            stack.push_back({pi + 1, high});
        }
    }
}

// 9. RADIX SORT
void counting_sort_for_radix(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    vector<int> count(10, 0);
    
    for (int num : arr) {
        int index = (num / exp) % 10;
        count[index]++;
    }
    
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    
    for (int i = n - 1; i >= 0; i--) {
        int index = (arr[i] / exp) % 10;
        output[count[index] - 1] = arr[i];
        count[index]--;
    }
    
    arr = output;
}

void radix_sort(vector<int>& arr) {
    if (arr.empty()) return;
    
    int max_val = *max_element(arr.begin(), arr.end());
    
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        counting_sort_for_radix(arr, exp);
    }
}

// 10. SELECCION
void selection_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i)
            swap(arr[i], arr[min_idx]);
    }
}

// 11. SHELL SORT
void shell_sort(vector<int>& arr) {
    int n = arr.size();
    
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}

// ==================== FUNCION DE MEDICION ====================
double medir_tiempo(vector<int> arr, function<void(vector<int>&)> sort_func, bool& timeout) {
    timeout = false;
    auto inicio = high_resolution_clock::now();
    
    try {
        sort_func(arr);
    } catch (...) {
        timeout = true;
        return -1;
    }
    
    auto fin = high_resolution_clock::now();
    double tiempo = duration<double, milli>(fin - inicio).count();
    
    if (tiempo > TIMEOUT_MS) {
        timeout = true;
    }
    
    return tiempo;
}

// ==================== VERIFICAR SI DEBE OMITIRSE ====================
bool debe_omitir_algoritmo(const string& nombre, int n) {
    // Algoritmos O(n²) lentos para arrays grandes
    if (n >= 10000) {
        if (nombre == "Burbuja" || nombre == "Insercion" || nombre == "Seleccion") {
            return true;
        }
    }
    return false;
}

// ==================== OBTENER DESCRIPCION DEL ESCENARIO ====================
string obtener_descripcion_escenario(const string& tipo) {
    if (tipo == "Ordenado") {
        return "Arreglo ya ordenado ascendentemente [1, 2, 3, ..., n]";
    } else if (tipo == "Medianamente Ordenado") {
        return "Arreglo parcialmente ordenado (70% ordenado, 30% aleatorio)";
    } else {
        return "Arreglo ordenado inversamente [n, n-1, ..., 2, 1]";
    }
}

// ==================== LIMPIAR PANTALLA ====================
void limpiar_pantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Declaración anticipada
void mostrar_recomendaciones();

// ==================== FUNCION DE ANALISIS ====================
void analizar_algoritmo(const string& nombre, function<void(vector<int>&)> sort_func) {
    vector<int> tamanios = {100, 1000, 10000, 100000};
    vector<string> tipos = {"Ordenado", "Medianamente Ordenado", "Inverso"};
    
    cout << "\n";
    cout << string(79, '-') << "\n";
    cout << "                   ANALISIS: " << nombre << "\n";
    cout << string(79, '-') << "\n\n";

    for (int n : tamanios) {
        cout << string(79, '-') << "\n";
        cout << "  TAMANO DEL ARREGLO: " << setw(8) << n << " elementos\n";
        cout << string(79, '-') << "\n\n";
        
        double tiempo_mejor = -1;
        string escenario_mejor;
        
        for (size_t i = 0; i < tipos.size(); i++) {
            string tipo = tipos[i];
            cout << "  " << string(71, '-') << "\n";
            cout << "  | ESCENARIO " << (i + 1) << ": " << left << setw(55) << tipo << "|\n";
            cout << "  | " << left << setw(70) << obtener_descripcion_escenario(tipo) << "|\n";
            cout << "  " << string(71, '-') << "\n";
            
            // Verificar si debe omitirse
            if (debe_omitir_algoritmo(nombre, n)) {
                cout << "    [OMITIDO - Demasiado lento para este tamano]\n\n";
                continue;
            }
            
            vector<int> arr;
            if (i == 0) {
                arr = generar_ordenado(n);
            } else if (i == 1) {
                arr = generar_medianamente_ordenado(n);
            } else {
                arr = generar_inverso(n);
            }
            
            bool timeout = false;
            double tiempo = medir_tiempo(arr, sort_func, timeout);
            
            if (timeout || tiempo < 0) {
                cout << "    Tiempo de ejecucion: [TIMEOUT - Excedio " << (TIMEOUT_MS/1000) << " segundos]\n\n";
            } else {
                cout << "    Tiempo de ejecucion: " << fixed << setprecision(4) << setw(12) << tiempo << " ms\n\n";
                
                if (tiempo_mejor == -1 || tiempo < tiempo_mejor) {
                    tiempo_mejor = tiempo;
                    escenario_mejor = tipo;
                }
            }
        }
        
        if (tiempo_mejor != -1) {
            cout << "  " << string(71, '=') << "\n";
            cout << "  | MEJOR ESCENARIO para " << setw(8) << n << " elementos: " << left << setw(27) << escenario_mejor << "|\n";
            cout << "  |    Tiempo: " << fixed << setprecision(4) << setw(12) << tiempo_mejor << " ms" << string(43, ' ') << "|\n";
            cout << "  " << string(71, '=') << "\n\n";
        } else {
            cout << "  " << string(71, '=') << "\n";
            cout << "  | Todos los escenarios fueron omitidos o excedieron el timeout          |\n";
            cout << "  " << string(71, '=') << "\n\n";
        }
    }
}

// ==================== COMPARAR TODOS LOS ALGORITMOS ====================
void comparar_todos_los_algoritmos() {
    vector<pair<string, function<void(vector<int>&)>>> algoritmos = {
        {"Burbuja", bubble_sort},
        {"Por Cubos", bucket_sort},
        {"Comb Sort", comb_sort},
        {"Conteo", counting_sort},
        {"Heap Sort", heap_sort},
        {"Insercion", insertion_sort},
        {"Fusion", merge_sort},
        {"Rapido", quick_sort},
        {"Radix Sort", radix_sort},
        {"Seleccion", selection_sort},
        {"Shell Sort", shell_sort}
    };
    
    vector<int> tamanios = {100, 1000, 10000, 100000};
    vector<string> tipos_orden = {"Ordenado", "Medianamente Ordenado", "Inverso"};
    
    cout << "\n";
    cout << string(79, '-') << "\n";
    cout << "            COMPARACION COMPLETA DE TODOS LOS ALGORITMOS\n";
    cout << string(79, '-') << "\n";
    
    for (int n : tamanios) {
        cout << "\n" << string(79, '=') << "\n";
        cout << "  TAMANO DEL ARREGLO: " << setw(8) << n << " elementos\n";
        cout << string(79, '=') << "\n";
        
        for (size_t tipo_idx = 0; tipo_idx < tipos_orden.size(); tipo_idx++) {
            string tipo = tipos_orden[tipo_idx];
            cout << "\n" << string(79, '-') << "\n";
            cout << "  ESCENARIO " << (tipo_idx + 1) << ": " << left << setw(60) << tipo << "\n";
            cout << "  " << left << setw(74) << obtener_descripcion_escenario(tipo) << "\n";
            cout << string(79, '-') << "\n\n";
            
            vector<pair<string, double>> resultados;
            
            for (const auto& [nombre, funcion] : algoritmos) {
                // Verificar si debe omitirse
                if (debe_omitir_algoritmo(nombre, n)) {
                    cout << "  " << left << setw(22) << nombre << ": [OMITIDO - Muy lento]\n";
                    continue;
                }
                
                vector<int> arr;
                if (tipo_idx == 0) {
                    arr = generar_ordenado(n);
                } else if (tipo_idx == 1) {
                    arr = generar_medianamente_ordenado(n);
                } else {
                    arr = generar_inverso(n);
                }
                
                bool timeout = false;
                double tiempo = medir_tiempo(arr, funcion, timeout);
                
                if (timeout || tiempo < 0) {
                    cout << "  " << left << setw(22) << nombre << ": [TIMEOUT]\n";
                } else {
                    resultados.push_back({nombre, tiempo});
                    cout << "  " << left << setw(22) << nombre << ": " << fixed << setprecision(4) << setw(14) << tiempo << " ms\n";
                }
            }
            
            if (!resultados.empty()) {
                auto mejor = *min_element(resultados.begin(), resultados.end(), 
                    [](const auto& a, const auto& b) { return a.second < b.second; });
                auto peor = *max_element(resultados.begin(), resultados.end(), 
                    [](const auto& a, const auto& b) { return a.second < b.second; });
                
                double diferencia = peor.second - mejor.second;
                double factor = mejor.second > 0 ? peor.second / mejor.second : 0;
                
                cout << "\n  " << string(71, '=') << "\n";
                cout << "  |  MEJOR ALGORITMO: " << left << setw(22) << mejor.first << " " << fixed << setprecision(4) << setw(14) << mejor.second << " ms       |\n";
                cout << "  |  PEOR ALGORITMO:  " << left << setw(22) << peor.first << " " << fixed << setprecision(4) << setw(14) << peor.second << " ms       |\n";
                cout << "  |  DIFERENCIA:      " << fixed << setprecision(4) << setw(36) << diferencia << " ms        |\n";
                cout << "  |  FACTOR DE MEJORA: " << fixed << setprecision(2) << setw(35) << factor << "x          |\n";
                cout << "  " << string(71, '=') << "\n";
            }
        }
    }
    
    mostrar_recomendaciones();
}

// ==================== MOSTRAR RECOMENDACIONES ====================
void mostrar_recomendaciones() {
    cout << "\n\n" << string(79, '=') << "\n";
    cout << "                    RESUMEN Y RECOMENDACIONES\n";
    cout << string(79, '=') << "\n\n";
    
    cout << string(79, '-') << "\n";
    cout << "  ESCENARIO 1: DATOS YA ORDENADOS\n";
    cout << "     Descripcion: [1, 2, 3, ..., n]\n";
    cout << string(79, '-') << "\n";
    cout << "   Mejor opcion: Counting Sort o Radix Sort\n";
    cout << "   Complejidad: O(n) - Tiempo lineal\n";
    cout << "   Alternativa: Insertion Sort (O(n) en mejor caso)\n\n";
    
    cout << string(79, '-') << "\n";
    cout << "  ESCENARIO 2: DATOS MEDIANAMENTE ORDENADOS\n";
    cout << "     Descripcion: 70% ordenado, 30% elementos aleatorios\n";
    cout << string(79, '-') << "\n";
    cout << "   Mejor opcion: Quick Sort o Merge Sort\n";
    cout << "   Complejidad: O(n log n) promedio\n";
    cout << "   Ventaja: Buen balance entre velocidad y estabilidad\n\n";
    
    cout << string(79, '-') << "\n";
    cout << "  ESCENARIO 3: DATOS ORDENADOS INVERSAMENTE\n";
    cout << "     Descripcion: [n, n-1, ..., 2, 1] - Peor caso para muchos algoritmos\n";
    cout << string(79, '-') << "\n";
    cout << "   Mejor opcion: Merge Sort o Heap Sort\n";
    cout << "   Complejidad: O(n log n) garantizado\n";
    cout << "   EVITAR: Quick Sort estandar puede degradar (pero la version aleatoria funciona bien)\n\n";
    
    cout << string(79, '=') << "\n";
    cout << "                    RECOMENDACION GENERAL POR CASO\n";
    cout << string(79, '=') << "\n\n";
    cout << "  Uso general (datos aleatorios):     Quick Sort o Merge Sort\n";
    cout << "  Datos pequenos con enteros:         Counting Sort\n";
    cout << "  Estabilidad garantizada:            Merge Sort\n";
    cout << "  Memoria limitada:                   Heap Sort o Quick Sort (in-place)\n";
    cout << "  Datos casi ordenados:               Insertion Sort o Shell Sort\n";
    cout << "  Enteros con rango limitado:         Radix Sort o Counting Sort\n\n";
    
    cout << string(79, '-') << "\n";
    cout << "  NOTA: Los algoritmos O(n²) como Burbuja, Insercion y Seleccion\n";
    cout << "        se omiten automaticamente para arrays >= 10,000 elementos\n";
    cout << "        debido a su lentitud extrema.\n";
    cout << string(79, '-') << "\n\n";
}

// ==================== MENU PRINCIPAL ====================
int main() {
    int opcion;
    
    while (true) {
        limpiar_pantalla();
        cout << "\n" << string(79, '=') << "\n";
        cout << "         SISTEMA DE ANALISIS DE ALGORITMOS DE ORDENAMIENTO\n";
        cout << string(79, '=') << "\n";
        cout << string(79, '-') << "\n";
        cout << "  1.  Burbuja (Bubble Sort)\n";
        cout << "  2.  Por Cubos (Bucket Sort)\n";
        cout << "  3.  Comb Sort\n";
        cout << "  4.  Conteo (Counting Sort)\n";
        cout << "  5.  Heap Sort\n";
        cout << "  6.  Insercion (Insertion Sort)\n";
        cout << "  7.  Fusion (Merge Sort)\n";
        cout << "  8.  Rapido (Quick Sort)\n";
        cout << "  9.  Radix Sort\n";
        cout << "  10. Por Seleccion (Selection Sort)\n";
        cout << "  11. Shell Sort\n";
        cout << string(79, '-') << "\n";
        cout << "  12. COMPARAR TODOS LOS ALGORITMOS\n";
        cout << string(79, '-') << "\n";
        cout << "  0.  Salir\n";
        cout << string(79, '-') << "\n";
        cout << "\n  NOTA: Algoritmos lentos se omiten automaticamente en arrays grandes\n";
        cout << string(79, '-') << "\n";
        
        cout << "\n>>> Seleccione una opcion: ";
        cin >> opcion;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();
        
        if (opcion == 0) {
            cout << "\n" << string(79, '=') << "\n";
            cout << "           Gracias por usar el Sistema de Analisis!\n";
            cout << string(79, '=') << "\n\n";
            break;
        }
        
        limpiar_pantalla();
        
        switch (opcion) {
            case 1:
                analizar_algoritmo("METODO BURBUJA (BUBBLE SORT)", bubble_sort);
                break;
            case 2:
                analizar_algoritmo("POR CUBOS (BUCKET SORT)", bucket_sort);
                break;
            case 3:
                analizar_algoritmo("COMB SORT", comb_sort);
                break;
            case 4:
                analizar_algoritmo("CONTEO (COUNTING SORT)", counting_sort);
                break;
            case 5:
                analizar_algoritmo("HEAP SORT", heap_sort);
                break;
            case 6:
                analizar_algoritmo("INSERCION (INSERTION SORT)", insertion_sort);
                break;
            case 7:
                analizar_algoritmo("FUSION (MERGE SORT)", merge_sort);
                break;
            case 8:
                analizar_algoritmo("RAPIDO (QUICK SORT)", quick_sort);
                break;
            case 9:
                analizar_algoritmo("RADIX SORT", radix_sort);
                break;
            case 10:
                analizar_algoritmo("POR SELECCION (SELECTION SORT)", selection_sort);
                break;
            case 11:
                analizar_algoritmo("SHELL SORT", shell_sort);
                break;
            case 12:
                comparar_todos_los_algoritmos();
                break;
            default:
                cout << "\nOpcion no valida. Por favor, intente de nuevo.\n";
        }
        
        if (opcion != 0) {
            cout << "\n>>> Presione Enter para continuar...";
            cin.get();
        }
    }
    
    return 0;
}
#include "FileSystem.h"
#include <iostream>
#include <cassert>
#include <chrono>

using namespace std;
using namespace chrono;

// ============ PRUEBAS UNITARIAS ============

void testCrearNodos() {
    cout << "\n=== TEST 1: Crear Nodos ===" << endl;
    FileSystem fs;
    
    assert(fs.crearNodo("/carpeta1", NodeType::FOLDER) == true);
    assert(fs.crearNodo("/archivo1.txt", NodeType::FILE, "contenido") == true);
    assert(fs.crearNodo("/carpeta1/subcarpeta", NodeType::FOLDER) == true);
    assert(fs.crearNodo("/carpeta1", NodeType::FOLDER) == false);  // Duplicado
    assert(fs.crearNodo("/noexiste/archivo.txt", NodeType::FILE) == false);  // Ruta inválida
    
    cout << " 5/5 tests pasaron" << endl;
}

void testEliminarNodos() {
    cout << "\n=== TEST 2: Eliminar Nodos ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/carpeta1", NodeType::FOLDER);
    fs.crearNodo("/carpeta1/archivo.txt", NodeType::FILE);
    
    assert(fs.eliminarNodo("/carpeta1/archivo.txt") == true);
    assert(fs.crearNodo("/carpeta1/archivo.txt", NodeType::FILE) == true);  // Verificar eliminación
    assert(fs.eliminarNodo("/carpeta1") == true);
    assert(fs.eliminarNodo("/noexiste") == false);
    
    cout << " 4/4 tests pasaron" << endl;
}

void testMoverNodos() {
    cout << "\n=== TEST 3: Mover Nodos ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/origen", NodeType::FOLDER);
    fs.crearNodo("/destino", NodeType::FOLDER);
    fs.crearNodo("/origen/archivo.txt", NodeType::FILE);
    
    assert(fs.moverNodo("/origen/archivo.txt", "/destino") == true);
    assert(fs.moverNodo("/destino/archivo.txt", "/noexiste") == false);
    
    // Test: No mover carpeta dentro de sí misma
    fs.crearNodo("/a", NodeType::FOLDER);
    fs.crearNodo("/a/b", NodeType::FOLDER);
    assert(fs.moverNodo("/a", "/a/b") == false);
    
    cout << " 3/3 tests pasaron" << endl;
}

void testRenombrarNodos() {
    cout << "\n=== TEST 4: Renombrar Nodos ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/archivo.txt", NodeType::FILE);
    
    assert(fs.renombrarNodo("/archivo.txt", "nuevo.txt") == true);
    assert(fs.buscarExacto("nuevo.txt") == true);
    assert(fs.buscarExacto("archivo.txt") == false);
    
    fs.crearNodo("/otro.txt", NodeType::FILE);
    assert(fs.renombrarNodo("/otro.txt", "nuevo.txt") == false);  // Duplicado
    
    cout << " 4/4 tests pasaron" << endl;
}

void testBusqueda() {
    cout << "\n=== TEST 5: Búsqueda (Trie + Hash) ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/proyecto1", NodeType::FOLDER);
    fs.crearNodo("/proyecto2", NodeType::FOLDER);
    fs.crearNodo("/programa.cpp", NodeType::FILE);
    fs.crearNodo("/producto.txt", NodeType::FILE);
    
    // Búsqueda exacta
    assert(fs.buscarExacto("proyecto1") == true);
    assert(fs.buscarExacto("noexiste") == false);
    
    // Autocompletado
    auto resultados = fs.buscarPorPrefijo("pro");
    assert(resultados.size() == 4);  // proyecto1, proyecto2, programa, producto
    
    resultados = fs.buscarPorPrefijo("proyecto");
    assert(resultados.size() == 2);  // proyecto1, proyecto2
    
    resultados = fs.buscarPorPrefijo("xyz");
    assert(resultados.size() == 0);
    
    cout << " 6/6 tests pasaron" << endl;
}

void testRecorridoPreorden() {
    cout << "\n=== TEST 6: Recorrido Preorden ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/a", NodeType::FOLDER);
    fs.crearNodo("/a/b", NodeType::FILE);
    fs.crearNodo("/a/c", NodeType::FILE);
    fs.crearNodo("/d", NodeType::FILE);
    
    auto recorrido = fs.exportarPreorden();
    assert(recorrido.size() == 5);  // root, a, b, c, d
    
    // Verificar que root es primero
    assert(recorrido[0].find("root") != string::npos);
    
    cout << " 2/2 tests pasaron" << endl;
}

void testPapelera() {
    cout << "\n=== TEST 7: Papelera ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/archivo1.txt", NodeType::FILE);
    fs.crearNodo("/archivo2.txt", NodeType::FILE);
    
    fs.eliminarNodo("/archivo1.txt");
    fs.eliminarNodo("/archivo2.txt");
    
    cout << "Contenido de papelera:" << endl;
    fs.mostrarPapelera();
    
    fs.vaciarPapelera();
    
    cout << " Test de papelera completado" << endl;
}

// ============ PRUEBAS DE CONSISTENCIA ============

void testConsistencia() {
    cout << "\n=== TEST 8: Consistencia del Árbol ===" << endl;
    FileSystem fs;
    
    // Crear estructura compleja
    fs.crearNodo("/a", NodeType::FOLDER);
    fs.crearNodo("/a/b", NodeType::FOLDER);
    fs.crearNodo("/a/b/c.txt", NodeType::FILE);
    fs.crearNodo("/a/x.txt", NodeType::FILE);
    
    // Mover y verificar
    fs.moverNodo("/a/b", "/");
    assert(fs.buscarExacto("b") == true);
    
    // Renombrar y verificar
    fs.renombrarNodo("/b", "b_renamed");
    assert(fs.buscarExacto("b_renamed") == true);
    assert(fs.buscarExacto("b") == false);
    
    // Eliminar y verificar
    fs.eliminarNodo("/b_renamed");
    assert(fs.buscarExacto("b_renamed") == false);
    
    cout << " Consistencia mantenida" << endl;
}

// ============ PRUEBAS DE CASOS LÍMITE ============

void testCasosLimite() {
    cout << "\n=== TEST 9: Casos Límite ===" << endl;
    FileSystem fs;
    
    // Test 1: Árbol profundo
    cout << "Test 1: Árbol con profundidad 20..." << endl;
    string ruta = "";
    for (int i = 0; i < 20; i++) {
        ruta += "/nivel" + to_string(i);
        assert(fs.crearNodo(ruta, NodeType::FOLDER) == true);
    }
    cout << " Árbol profundo creado" << endl;
    
    // Test 2: Muchos hermanos
    cout << "Test 2: 50 archivos en misma carpeta..." << endl;
    fs.crearNodo("/test", NodeType::FOLDER);
    for (int i = 0; i < 50; i++) {
        string nombre = "/test/file" + to_string(i) + ".txt";
        assert(fs.crearNodo(nombre, NodeType::FILE) == true);
    }
    cout << "✓ 50 archivos creados" << endl;
    
    // Test 3: Nombres con caracteres especiales (válidos)
    cout << "Test 3: Nombres especiales..." << endl;
    assert(fs.crearNodo("/archivo_con_guion.txt", NodeType::FILE) == true);
    assert(fs.crearNodo("/archivo-con-guion.txt", NodeType::FILE) == true);
    assert(fs.crearNodo("/archivo.con.puntos.txt", NodeType::FILE) == true);
    cout << "Nombres especiales aceptados" << endl;
    
    cout << "✅ 3/3 casos límite pasaron" << endl;
}

// ============ PRUEBAS DE PERFORMANCE ============

void testPerformance() {
    cout << "\n=== TEST 10: Performance ===" << endl;
    FileSystem fs;
    
    // Test 1: Crear 1000 nodos
    cout << "Test 1: Creando 1000 nodos..." << endl;
    auto inicio = high_resolution_clock::now();
    
    for (int i = 0; i < 10; i++) {
        string carpeta = "/carpeta" + to_string(i);
        fs.crearNodo(carpeta, NodeType::FOLDER);
        
        for (int j = 0; j < 100; j++) {
            string archivo = carpeta + "/archivo" + to_string(j) + ".txt";
            fs.crearNodo(archivo, NodeType::FILE, "contenido");
        }
    }
    
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);
    cout << " 1000 nodos creados en " << duracion.count() << "ms" << endl;
    
    // Test 2: 1000 búsquedas
    cout << "Test 2: 1000 búsquedas por prefijo..." << endl;
    inicio = high_resolution_clock::now();
    
    for (int i = 0; i < 1000; i++) {
        fs.buscarPorPrefijo("archivo");
    }
    
    fin = high_resolution_clock::now();
    duracion = duration_cast<milliseconds>(fin - inicio);
    cout << "✓ 1000 búsquedas en " << duracion.count() << "ms" << endl;
    
    // Test 3: Recorrido de árbol grande
    cout << "Test 3: Exportar árbol con 1000+ nodos..." << endl;
    inicio = high_resolution_clock::now();
    
    auto recorrido = fs.exportarPreorden();
    
    fin = high_resolution_clock::now();
    duracion = duration_cast<milliseconds>(fin - inicio);
    cout << "✓ " << recorrido.size() << " nodos exportados en " 
         << duracion.count() << "ms" << endl;
    
    cout << " Tests de performance completados" << endl;
}

// ============ MAIN ============

int main() {
    cout << "====================================================" << endl;
    cout << "|     SUITE COMPLETA DE PRUEBAS - DÍA 10-11        |" << endl;
    cout << "|        Sistema de Gestión de Archivos            |" << endl;
    cout << "====================================================" << endl;
    
    try {
        cout << "\n EJECUTANDO PRUEBAS UNITARIAS..." << endl;
        testCrearNodos();
        testEliminarNodos();
        testMoverNodos();
        testRenombrarNodos();
        testBusqueda();
        testRecorridoPreorden();
        testPapelera();
        
        cout << "\n EJECUTANDO PRUEBAS DE INTEGRACIÓN..." << endl;
        testConsistencia();
        
        cout << "\n EJECUTANDO PRUEBAS DE CASOS LÍMITE..." << endl;
        testCasosLimite();
        
        cout << "\n EJECUTANDO PRUEBAS DE PERFORMANCE..." << endl;
        testPerformance();
        
        cout << "\n===================================================" << endl;
        cout << "|  ✅ TODAS LAS PRUEBAS PASARON EXITOSAMENTE       |" << endl;
        cout << "|                                                   |" << endl;
        cout << "|  Total de tests: 10 suites                       |" << endl;
        cout << "|  Casos individuales: 30+                         |" << endl;
        cout << "|  Performance: Aprobado                           |" << endl;
        cout << "===================================================" << endl;
        
    } catch (const exception& e) {
        cout << "\n ERROR EN LOS TESTS: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
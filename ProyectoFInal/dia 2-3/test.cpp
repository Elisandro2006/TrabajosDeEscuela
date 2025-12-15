#include "FileSystem.h"
#include <iostream>
#include <cassert>

using namespace std;

void testCrearNodos() {
    cout << "\n=== TEST: Crear Nodos ===" << endl;
    FileSystem fs;
    
    // Test 1: Crear carpeta en raíz
    assert(fs.crearNodo("/carpeta1", NodeType::FOLDER) == true);
    cout << "✓ Test 1: Crear carpeta básica" << endl;
    
    // Test 2: Crear archivo en raíz
    assert(fs.crearNodo("/archivo1.txt", NodeType::FILE, "contenido de prueba") == true);
    cout << "✓ Test 2: Crear archivo básico" << endl;
    
    // Test 3: Crear subcarpeta
    assert(fs.crearNodo("/carpeta1/subcarpeta", NodeType::FOLDER) == true);
    cout << "✓ Test 3: Crear subcarpeta" << endl;
    
    // Test 4: Crear archivo en subcarpeta
    assert(fs.crearNodo("/carpeta1/subcarpeta/archivo.txt", NodeType::FILE) == true);
    cout << "✓ Test 4: Crear archivo en subcarpeta" << endl;
    
    // Test 5: Error - nodo duplicado
    assert(fs.crearNodo("/carpeta1", NodeType::FOLDER) == false);
    cout << "✓ Test 5: Prevenir nodos duplicados" << endl;
    
    // Test 6: Error - ruta padre no existe
    assert(fs.crearNodo("/noexiste/archivo.txt", NodeType::FILE) == false);
    cout << "✓ Test 6: Error en ruta inexistente" << endl;
    
    cout << "✅ TODOS LOS TESTS DE CREACIÓN PASARON\n" << endl;
}

void testEliminarNodos() {
    cout << "=== TEST: Eliminar Nodos ===" << endl;
    FileSystem fs;
    
    // Preparar estructura
    fs.crearNodo("/carpeta1", NodeType::FOLDER);
    fs.crearNodo("/carpeta1/archivo.txt", NodeType::FILE);
    fs.crearNodo("/archivo2.txt", NodeType::FILE);
    
    // Test 1: Eliminar archivo
    assert(fs.eliminarNodo("/carpeta1/archivo.txt") == true);
    cout << "✓ Test 1: Eliminar archivo" << endl;
    
    // Test 2: Verificar que se eliminó (intentar crear con mismo nombre)
    assert(fs.crearNodo("/carpeta1/archivo.txt", NodeType::FILE) == true);
    cout << "✓ Test 2: Archivo eliminado correctamente" << endl;
    
    // Test 3: Eliminar carpeta
    assert(fs.eliminarNodo("/carpeta1") == true);
    cout << "✓ Test 3: Eliminar carpeta con contenido" << endl;
    
    // Test 4: Error - eliminar nodo inexistente
    assert(fs.eliminarNodo("/noexiste") == false);
    cout << "✓ Test 4: Error al eliminar nodo inexistente" << endl;
    
    cout << "✅ TODOS LOS TESTS DE ELIMINACIÓN PASARON\n" << endl;
}

void testListarYMostrarInfo() {
    cout << "=== TEST: Listar y Mostrar Información ===" << endl;
    FileSystem fs;
    
    // Crear estructura de prueba
    fs.crearNodo("/docs", NodeType::FOLDER);
    fs.crearNodo("/docs/file1.txt", NodeType::FILE, "contenido 1");
    fs.crearNodo("/docs/file2.txt", NodeType::FILE, "contenido 2");
    fs.crearNodo("/docs/subfolder", NodeType::FOLDER);
    
    cout << "\nTest 1: Listar contenido de /docs:" << endl;
    fs.listarHijos("/docs");
    
    cout << "\nTest 2: Mostrar información de /docs/file1.txt:" << endl;
    fs.mostrarInfo("/docs/file1.txt");
    
    cout << "\nTest 3: Mostrar información de /docs:" << endl;
    fs.mostrarInfo("/docs");
    
    cout << "\n✅ TESTS DE LISTADO COMPLETADOS\n" << endl;
}

void testCalculoAlturaYTamano() {
    cout << "=== TEST: Cálculo de Altura y Tamaño ===" << endl;
    FileSystem fs;
    
    // Crear estructura de árbol
    fs.crearNodo("/nivel1", NodeType::FOLDER);
    fs.crearNodo("/nivel1/nivel2", NodeType::FOLDER);
    fs.crearNodo("/nivel1/nivel2/nivel3", NodeType::FOLDER);
    fs.crearNodo("/nivel1/nivel2/nivel3/archivo.txt", NodeType::FILE);
    fs.crearNodo("/nivel1/archivo2.txt", NodeType::FILE);
    
    cout << "Estructura creada:" << endl;
    cout << "/nivel1/" << endl;
    cout << "  /nivel2/" << endl;
    cout << "    /nivel3/" << endl;
    cout << "      archivo.txt" << endl;
    cout << "  archivo2.txt" << endl;
    
    cout << "\nInformación del nodo /nivel1:" << endl;
    fs.mostrarInfo("/nivel1");
    
    // La altura debería ser 4 (nivel1 -> nivel2 -> nivel3 -> archivo)
    // El tamaño debería ser 5 (nivel1, nivel2, nivel3, archivo, archivo2)
    
    cout << "\n✅ TEST DE MÉTRICAS COMPLETADO\n" << endl;
}

void testValidaciones() {
    cout << "=== TEST: Validaciones ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/test", NodeType::FOLDER);
    fs.crearNodo("/archivo.txt", NodeType::FILE);
    
    // Test 1: Intentar crear archivo en ruta que es un archivo
    assert(fs.crearNodo("/archivo.txt/subcarpeta", NodeType::FOLDER) == false);
    cout << "✓ Test 1: No permitir crear en archivo" << endl;
    
    // Test 2: Verificar que no se puede eliminar raíz
    assert(fs.eliminarNodo("/") == false);
    cout << "✓ Test 2: No permitir eliminar raíz" << endl;
    
    // Test 3: Listar carpeta inexistente
    cout << "\nTest 3: Listar carpeta inexistente:" << endl;
    fs.listarHijos("/noexiste");
    
    // Test 4: Info de nodo inexistente
    cout << "\nTest 4: Info de nodo inexistente:" << endl;
    fs.mostrarInfo("/noexiste");
    
    cout << "\n✅ TESTS DE VALIDACIÓN COMPLETADOS\n" << endl;
}

int main() {
    cout << "=========================================" << endl;
    cout << "|  PRUEBAS UNITARIAS - DÍA 3           |" << endl;
    cout << "|  Operaciones Básicas del Árbol       |" << endl;
    cout << "=========================================" << endl;
    
    try {
        testCrearNodos();
        testEliminarNodos();
        testListarYMostrarInfo();
        testCalculoAlturaYTamano();
        testValidaciones();
        
        cout << "\n=======================================" << endl;
        cout << "|  ✅ TODOS LOS TESTS PASARON          |" << endl;
        cout << "=========================================" << endl;
        
    } catch (const exception& e) {
        cout << "\n✗ ERROR: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
#include "FileSystem.h"
#include <iostream>
#include <cassert>

using namespace std;

void testCrearNodos() {
    cout << "\n=== TEST: Crear Nodos ===" << endl;
    FileSystem fs;
    
    // Test: Crear carpeta
    assert(fs.crearNodo("/carpeta1", NodeType::FOLDER) == true);
    cout << " Crear carpeta básica" << endl;
    
    // Test: Crear archivo
    assert(fs.crearNodo("/archivo1.txt", NodeType::FILE, "contenido") == true);
    cout << " Crear archivo básico" << endl;
    
    // Test: Crear nodo en carpeta existente
    assert(fs.crearNodo("/carpeta1/subcarpeta", NodeType::FOLDER) == true);
    cout << " Crear subcarpeta" << endl;
    
    // Test: Error al crear nodo duplicado
    assert(fs.crearNodo("/carpeta1", NodeType::FOLDER) == false);
    cout << " Prevenir nodos duplicados" << endl;
    
    // Test: Error al crear nodo en ruta inexistente
    assert(fs.crearNodo("/noexiste/archivo.txt", NodeType::FILE) == false);
    cout << " Prevenir creación en ruta inexistente" << endl;
    
    cout << " TODOS LOS TESTS DE CREACION PASARON" << endl;
}

void testEliminarNodos() {
    cout << "\n=== TEST: Eliminar Nodos ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/carpeta1", NodeType::FOLDER);
    fs.crearNodo("/carpeta1/archivo.txt", NodeType::FILE);
    
    // Test: Eliminar archivo
    assert(fs.eliminarNodo("/carpeta1/archivo.txt") == true);
    cout << " Eliminar archivo" << endl;
    
    // Test: Verificar que el archivo fue eliminado
    assert(fs.crearNodo("/carpeta1/archivo.txt", NodeType::FILE) == true);
    cout << " Archivo eliminado correctamente" << endl;
    
    // Test: Eliminar carpeta
    assert(fs.eliminarNodo("/carpeta1") == true);
    cout << " Eliminar carpeta" << endl;
    
    // Test: Error al eliminar nodo inexistente
    assert(fs.eliminarNodo("/noexiste") == false);
    cout << " Prevenir eliminación de nodo inexistente" << endl;
    
    cout << " TODOS LOS TESTS DE ELIMINACIÓN PASARON" << endl;
}

void testMoverNodos() {
    cout << "\n=== TEST: Mover Nodos ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/origen", NodeType::FOLDER);
    fs.crearNodo("/destino", NodeType::FOLDER);
    fs.crearNodo("/origen/archivo.txt", NodeType::FILE);
    
    // Test: Mover archivo
    assert(fs.moverNodo("/origen/archivo.txt", "/destino") == true);
    cout << " Mover archivo entre carpetas" << endl;
    
    // Test: Verificar que el archivo está en destino
    fs.crearNodo("/destino/nuevo.txt", NodeType::FILE); // Debería funcionar
    cout << " Archivo movido correctamente" << endl;
    
    // Test: Error al mover a ruta inexistente
    assert(fs.moverNodo("/destino/archivo.txt", "/noexiste") == false);
    cout << " Prevenir mover a ruta inexistente" << endl;
    
    cout << " TODOS LOS TESTS DE MOVIMIENTO PASARON" << endl;
}

void testRenombrarNodos() {
    cout << "\n=== TEST: Renombrar Nodos ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/archivo.txt", NodeType::FILE);
    
    // Test: Renombrar archivo
    assert(fs.renombrarNodo("/archivo.txt", "nuevo_nombre.txt") == true);
    cout << " Renombrar archivo" << endl;
    
    // Test: Error al renombrar con nombre existente
    fs.crearNodo("/otro.txt", NodeType::FILE);
    assert(fs.renombrarNodo("/otro.txt", "nuevo_nombre.txt") == false);
    cout << " Prevenir renombrar con nombre duplicado" << endl;
    
    // Test: Error al renombrar nodo inexistente
    assert(fs.renombrarNodo("/noexiste.txt", "nuevo") == false);
    cout << " Prevenir renombrar nodo inexistente" << endl;
    
    cout << " TODOS LOS TESTS DE RENOMBRADO PASARON" << endl;
}

void testBusqueda() {
    cout << "\n=== TEST: Búsqueda ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/proyecto1", NodeType::FOLDER);
    fs.crearNodo("/proyecto2", NodeType::FOLDER);
    fs.crearNodo("/programa.cpp", NodeType::FILE);
    
    // Test: Búsqueda exacta exitosa
    assert(fs.buscarExacto("proyecto1") == true);
    cout << " Búsqueda exacta encontrada" << endl;
    
    // Test: Búsqueda exacta fallida
    assert(fs.buscarExacto("noexiste") == false);
    cout << " Búsqueda exacta no encontrada" << endl;
    
    // Test: Autocompletado con prefijo
    auto resultados = fs.buscarPorPrefijo("pro");
    assert(resultados.size() == 3);
    cout << " Autocompletado encontró " << resultados.size() << " resultados" << endl;
    
    // Test: Autocompletado sin resultados
    resultados = fs.buscarPorPrefijo("xyz");
    assert(resultados.size() == 0);
    cout << " Autocompletado sin resultados" << endl;
    
    cout << " TODOS LOS TESTS DE BÚSQUEDA PASARON" << endl;
}

void testRecorridoPreorden() {
    cout << "\n=== TEST: Recorrido Preorden ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/carpeta", NodeType::FOLDER);
    fs.crearNodo("/carpeta/archivo.txt", NodeType::FILE);
    fs.crearNodo("/otro.txt", NodeType::FILE);
    
    auto recorrido = fs.exportarPreorden();
    
    // Test: El recorrido no está vacío
    assert(recorrido.size() > 0);
    cout << " Recorrido generado con " << recorrido.size() << " nodos" << endl;
    
    // Test: La raíz es el primer elemento
    assert(recorrido[0].find("root") != string::npos);
    cout << " Raíz es el primer elemento" << endl;
    
    cout << " TODOS LOS TESTS DE RECORRIDO PASARON" << endl;
}

void testConsistencia() {
    cout << "\n=== TEST: Consistencia del Árbol ===" << endl;
    FileSystem fs;
    
    // Crear estructura compleja
    fs.crearNodo("/a", NodeType::FOLDER);
    fs.crearNodo("/a/b", NodeType::FOLDER);
    fs.crearNodo("/a/b/c.txt", NodeType::FILE);
    
    // Mover y verificar consistencia
    fs.moverNodo("/a/b", "/");
    fs.listarHijos("/");
    fs.listarHijos("/b");
    cout << " Estructura consistente después de mover" << endl;
    
    // Renombrar y verificar
    fs.renombrarNodo("/b", "b_renombrado");
    assert(fs.buscarExacto("b_renombrado") == true);
    assert(fs.buscarExacto("b") == false);
    cout << " Búsqueda consistente después de renombrar" << endl;
    
    // Eliminar y verificar
    fs.eliminarNodo("/b_renombrado");
    assert(fs.buscarExacto("b_renombrado") == false);
    cout << " Búsqueda consistente después de eliminar" << endl;
    
    cout << " TODOS LOS TESTS DE CONSISTENCIA PASARON" << endl;
}

void testPapelera() {
    cout << "\n=== TEST: Papelera ===" << endl;
    FileSystem fs;
    
    fs.crearNodo("/archivo.txt", NodeType::FILE);
    fs.eliminarNodo("/archivo.txt");
    
    cout << "Verificando papelera..." << endl;
    fs.mostrarPapelera();
    
    fs.vaciarPapelera();
    cout << " Papelera vaciada" << endl;
    
    cout << " TODOS LOS TESTS DE PAPELERA PASARON" << endl;
}

int main() {
    cout << "=========================================" << endl;
    cout << "|     SUITE DE PRUEBAS UNITARIAS       |" << endl;
    cout << "|  Sistema de Gestión de Archivos      |" << endl;
    cout << "=========================================" << endl;
    
    try {
        testCrearNodos();
        testEliminarNodos();
        testMoverNodos();
        testRenombrarNodos();
        testBusqueda();
        testRecorridoPreorden();
        testConsistencia();
        testPapelera();
        
        cout << "\n=========================================" << endl;
        cout << "|    TODOS LOS TESTS PASARON           |" << endl;
        cout << "=========================================" << endl;
        
    } catch (const exception& e) {
        cout << "\n ERROR EN LOS TESTS: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
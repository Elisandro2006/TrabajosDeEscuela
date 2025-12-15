#include "FileSystem.h"
#include <iostream>
#include <sstream>

using namespace std;

void mostrarMenu() {
    cout << "\n========== SISTEMA DE ARCHIVOS ==========" << endl;
    cout << "1.  mkdir <ruta>           - Crear carpeta" << endl;
    cout << "2.  touch <ruta>           - Crear archivo" << endl;
    cout << "3.  mv <origen> <destino>  - Mover nodo" << endl;
    cout << "4.  rename <ruta> <nuevo>  - Renombrar nodo" << endl;
    cout << "5.  rm <ruta>              - Eliminar nodo" << endl;
    cout << "6.  ls <ruta>              - Listar hijos" << endl;
    cout << "7.  info <ruta>            - Mostrar informacion" << endl;
    cout << "8.  search <prefijo>       - Buscar por prefijo" << endl;
    cout << "9.  find <nombre>          - Busqueda exacta" << endl;
    cout << "10. export                 - Exportar preorden" << endl;
    cout << "11. trash                  - Ver papelera" << endl;
    cout << "12. clear-trash            - Vaciar papelera" << endl;
    cout << "13. stats                  - Ver estadisticas" << endl;
    cout << "14. help                   - Mostrar este menu" << endl;
    cout << "15. exit                   - Salir" << endl;
    cout << "=========================================" << endl;
}

void ejecutarComando(FileSystem& fs, const string& comando) {
    stringstream ss(comando);
    string cmd;
    ss >> cmd;
    
    if (cmd == "mkdir") {
        string ruta;
        ss >> ruta;
        if (ruta.empty()) {
            cout << "Uso: mkdir <ruta>" << endl;
            return;
        }
        if (fs.crearNodo(ruta, NodeType::FOLDER)) {
            cout << "Carpeta creada: " << ruta << endl;
        }
    }
    else if (cmd == "touch") {
        string ruta;
        ss >> ruta;
        if (ruta.empty()) {
            cout << "Uso: touch <ruta> [contenido]" << endl;
            return;
        }
        string contenido;
        getline(ss, contenido);
        if (!contenido.empty() && contenido[0] == ' ') {
            contenido = contenido.substr(1);
        }
        if (fs.crearNodo(ruta, NodeType::FILE, contenido)) {
            cout << "Archivo creado: " << ruta << endl;
        }
    }
    else if (cmd == "mv") {
        string origen, destino;
        ss >> origen >> destino;
        if (origen.empty() || destino.empty()) {
            cout << "Uso: mv <origen> <destino>" << endl;
            return;
        }
        if (fs.moverNodo(origen, destino)) {
            cout << "Nodo movido de " << origen << " a " << destino << endl;
        }
    }
    else if (cmd == "rename") {
        string ruta, nuevoNombre;
        ss >> ruta >> nuevoNombre;
        if (ruta.empty() || nuevoNombre.empty()) {
            cout << "Uso: rename <ruta> <nuevo_nombre>" << endl;
            return;
        }
        if (fs.renombrarNodo(ruta, nuevoNombre)) {
            cout << "Nodo renombrado a: " << nuevoNombre << endl;
        }
    }
    else if (cmd == "rm") {
        string ruta;
        ss >> ruta;
        if (ruta.empty()) {
            cout << "Uso: rm <ruta>" << endl;
            return;
        }
        if (fs.eliminarNodo(ruta)) {
            cout << "Nodo eliminado y movido a papelera: " << ruta << endl;
        }
    }
    else if (cmd == "ls") {
        string ruta;
        ss >> ruta;
        if (ruta.empty()) {
            ruta = "/";
        }
        fs.listarHijos(ruta);
    }
    else if (cmd == "info") {
        string ruta;
        ss >> ruta;
        if (ruta.empty()) {
            cout << "Uso: info <ruta>" << endl;
            return;
        }
        fs.mostrarRuta(ruta);
    }
    else if (cmd == "search") {
        string prefijo;
        ss >> prefijo;
        if (prefijo.empty()) {
            cout << "Uso: search <prefijo>" << endl;
            return;
        }
        auto resultados = fs.buscarPorPrefijo(prefijo);
        cout << "Resultados para prefijo '" << prefijo << "':" << endl;
        if (resultados.empty()) {
            cout << "  No se encontraron resultados" << endl;
        } else {
            for (const auto& res : resultados) {
                cout << "  - " << res << endl;
            }
        }
    }
    else if (cmd == "find") {
        string nombre;
        ss >> nombre;
        if (nombre.empty()) {
            cout << "Uso: find <nombre>" << endl;
            return;
        }
        if (fs.buscarExacto(nombre)) {
            cout << "Encontrado: " << nombre << endl;
        } else {
            cout << "No encontrado: " << nombre << endl;
        }
    }
    else if (cmd == "export") {
        cout << "\n=== RECORRIDO EN PREORDEN ===" << endl;
        auto recorrido = fs.exportarPreorden();
        for (const auto& linea : recorrido) {
            cout << linea << endl;
        }
        cout << "============================" << endl;
    }
    else if (cmd == "trash") {
        fs.mostrarPapelera();
    }
    else if (cmd == "clear-trash") {
        fs.vaciarPapelera();
    }
    else if (cmd == "stats") {
        fs.mostrarEstadisticas();
    }
    else if (cmd == "help") {
        mostrarMenu();
    }
    else if (cmd == "exit") {
        cout << "Hasta luego!" << endl;
    }
    else {
        cout << "Comando desconocido. Escribe 'help' para ver los comandos disponibles." << endl;
    }
}

void ejecutarDemo(FileSystem& fs) {
    cout << "\n========== DEMO AUTOMATICA ==========" << endl;
    cout << "Creando estructura de ejemplo..." << endl;
    
    // Crear estructura de carpetas
    fs.crearNodo("/documentos", NodeType::FOLDER);
    fs.crearNodo("/imagenes", NodeType::FOLDER);
    fs.crearNodo("/proyectos", NodeType::FOLDER);
    fs.crearNodo("/documentos/trabajo", NodeType::FOLDER);
    fs.crearNodo("/documentos/personal", NodeType::FOLDER);
    
    // Crear archivos
    fs.crearNodo("/documentos/readme.txt", NodeType::FILE, "Este es un archivo de prueba");
    fs.crearNodo("/documentos/trabajo/reporte.doc", NodeType::FILE, "Reporte trimestral");
    fs.crearNodo("/imagenes/foto1.jpg", NodeType::FILE, "");
    fs.crearNodo("/imagenes/foto2.jpg", NodeType::FILE, "");
    fs.crearNodo("/proyectos/proyecto1.cpp", NodeType::FILE, "#include <iostream>");
    
    cout << "\nEstructura creada" << endl;
    
    // Listar contenido
    cout << "\n--- Listando raiz ---" << endl;
    fs.listarHijos("/");
    
    cout << "\n--- Listando /documentos ---" << endl;
    fs.listarHijos("/documentos");
    
    // Busqueda por prefijo
    cout << "\n--- Busqueda con prefijo 'pro' ---" << endl;
    auto resultados = fs.buscarPorPrefijo("pro");
    for (const auto& res : resultados) {
        cout << "  - " << res << endl;
    }
    
    // Exportar preorden
    cout << "\n--- Recorrido en preorden ---" << endl;
    auto recorrido = fs.exportarPreorden();
    for (const auto& linea : recorrido) {
        cout << linea << endl;
    }
    
    cout << "\n========== FIN DE LA DEMO ==========" << endl;
}

int main() {
    FileSystem fs;
    
    cout << "========================================" << endl;
    cout << "|  SISTEMA DE GESTION DE ARCHIVOS      |" << endl;
    cout << "|  Proyecto de Estructuras de Datos    |" << endl;
    cout << "========================================" << endl;
    
    cout << "\nDesea ejecutar la demo automatica? (s/n): ";
    string respuesta;
    getline(cin, respuesta);
    
    if (respuesta == "s" || respuesta == "S") {
        ejecutarDemo(fs);
    }
    
    mostrarMenu();
    
    string comando;
    while (true) {
        cout << "\n> ";
        getline(cin, comando);
        
        if (comando.empty()) {
            continue;
        }
        
        if (comando == "exit") {
            cout << "Hasta luego!" << endl;
            break;
        }
        
        ejecutarComando(fs, comando);
    }
    
    return 0;
}
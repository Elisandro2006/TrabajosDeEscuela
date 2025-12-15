#include "FileSystem.h"
#include <iostream>
#include <sstream>

using namespace std;

void mostrarBanner() {
    cout << "=========================================" << endl;
    cout << "|      SISTEMA DE GESTIÓN DE ARCHIVOS v1.0         |" << endl;
    cout << "|         Proyecto de Estructuras de Datos         |" << endl;
    cout << "=========================================" << endl;
}

void mostrarMenu() {
    cout << "\n========== COMANDOS DISPONIBLES ==========" << endl;
    cout << "ARCHIVOS:" << endl;
    cout << "  mkdir <ruta>           - Crear carpeta" << endl;
    cout << "  touch <ruta> [texto]   - Crear archivo" << endl;
    cout << "  mv <origen> <destino>  - Mover nodo" << endl;
    cout << "  rename <ruta> <nuevo>  - Renombrar" << endl;
    cout << "  rm <ruta>              - Eliminar" << endl;
    cout << "\nVISUALIZACIÓN:" << endl;
    cout << "  ls [ruta]              - Listar contenido" << endl;
    cout << "  info <ruta>            - Ver información" << endl;
    cout << "  export                 - Recorrido preorden" << endl;
    cout << "\nBÚSQUEDA:" << endl;
    cout << "  search <prefijo>       - Buscar por prefijo" << endl;
    cout << "  find <nombre>          - Búsqueda exacta" << endl;
    cout << "\nPAPELERA:" << endl;
    cout << "  trash                  - Ver papelera" << endl;
    cout << "  clear-trash            - Vaciar papelera" << endl;
    cout << "\nSISTEMA:" << endl;
    cout << "  help                   - Mostrar ayuda" << endl;
    cout << "  clear                  - Limpiar pantalla" << endl;
    cout << "  exit                   - Salir" << endl;
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
            cout << " Uso: mkdir <ruta>" << endl;
            cout << "   Ejemplo: mkdir /documentos" << endl;
            return;
        }
        if (fs.crearNodo(ruta, NodeType::FOLDER)) {
            cout << "✓ Carpeta creada: " << ruta << endl;
        }
    }
    else if (cmd == "touch") {
        string ruta;
        ss >> ruta;
        if (ruta.empty()) {
            cout << " Uso: touch <ruta> [contenido]" << endl;
            cout << "   Ejemplo: touch /archivo.txt Hola mundo" << endl;
            return;
        }
        string contenido;
        getline(ss, contenido);
        if (!contenido.empty() && contenido[0] == ' ') {
            contenido = contenido.substr(1);
        }
        if (fs.crearNodo(ruta, NodeType::FILE, contenido)) {
            cout << "✓ Archivo creado: " << ruta << endl;
        }
    }
    else if (cmd == "mv") {
        string origen, destino;
        ss >> origen >> destino;
        if (origen.empty() || destino.empty()) {
            cout << " Uso: mv <origen> <destino>" << endl;
            cout << "   Ejemplo: mv /archivo.txt /carpeta" << endl;
            return;
        }
        if (fs.moverNodo(origen, destino)) {
            cout << "✓ Movido: " << origen << " → " << destino << endl;
        }
    }
    else if (cmd == "rename") {
        string ruta, nuevoNombre;
        ss >> ruta >> nuevoNombre;
        if (ruta.empty() || nuevoNombre.empty()) {
            cout << " Uso: rename <ruta> <nuevo_nombre>" << endl;
            cout << "   Ejemplo: rename /viejo.txt nuevo.txt" << endl;
            return;
        }
        if (fs.renombrarNodo(ruta, nuevoNombre)) {
            cout << "✓ Renombrado a: " << nuevoNombre << endl;
        }
    }
    else if (cmd == "rm") {
        string ruta;
        ss >> ruta;
        if (ruta.empty()) {
            cout << " Uso: rm <ruta>" << endl;
            cout << "   Ejemplo: rm /archivo.txt" << endl;
            return;
        }
        
        cout << "  ¿Seguro que desea eliminar '" << ruta << "'? (s/n): ";
        string confirmacion;
        getline(cin, confirmacion);
        
        if (confirmacion == "s" || confirmacion == "S") {
            if (fs.eliminarNodo(ruta)) {
                cout << " Eliminado y movido a papelera: " << ruta << endl;
            }
        } else {
            cout << "Operación cancelada" << endl;
        }
    }
    else if (cmd == "ls") {
        string ruta;
        getline(ss, ruta);
        if (!ruta.empty() && ruta[0] == ' ') {
            ruta = ruta.substr(1);
        }
        if (ruta.empty()) {
            ruta = "/";
        }
        fs.listarHijos(ruta);
    }
    else if (cmd == "info") {
        string ruta;
        ss >> ruta;
        if (ruta.empty()) {
            cout << " Uso: info <ruta>" << endl;
            cout << "   Ejemplo: info /documentos" << endl;
            return;
        }
        fs.mostrarRuta(ruta);
    }
    else if (cmd == "search") {
        string prefijo;
        ss >> prefijo;
        if (prefijo.empty()) {
            cout << " Uso: search <prefijo>" << endl;
            cout << "   Ejemplo: search doc" << endl;
            return;
        }
        auto resultados = fs.buscarPorPrefijo(prefijo);
        cout << "🔍 Resultados para '" << prefijo << "':" << endl;
        if (resultados.empty()) {
            cout << "   No se encontraron resultados" << endl;
        } else {
            for (const auto& res : resultados) {
                cout << "   • " << res << endl;
            }
            cout << "   Total: " << resultados.size() << " resultado(s)" << endl;
        }
    }
    else if (cmd == "find") {
        string nombre;
        ss >> nombre;
        if (nombre.empty()) {
            cout << " Uso: find <nombre>" << endl;
            cout << "   Ejemplo: find archivo.txt" << endl;
            return;
        }
        if (fs.buscarExacto(nombre)) {
            cout << "✓ Encontrado: " << nombre << endl;
        } else {
            cout << "✗ No encontrado: " << nombre << endl;
        }
    }
    else if (cmd == "export") {
        cout << "\n=========================================" << endl;
        cout << "|     RECORRIDO EN PREORDEN             |" << endl;
        cout << "=========================================" << endl;
        auto recorrido = fs.exportarPreorden();
        for (const auto& linea : recorrido) {
            cout << linea << endl;
        }
        cout << "\nTotal de nodos: " << recorrido.size() << endl;
    }
    else if (cmd == "trash") {
        fs.mostrarPapelera();
    }
    else if (cmd == "clear-trash") {
        cout << "  ¿Seguro que desea vaciar la papelera? (s/n): ";
        string confirmacion;
        getline(cin, confirmacion);
        
        if (confirmacion == "s" || confirmacion == "S") {
            fs.vaciarPapelera();
        } else {
            cout << "Operación cancelada" << endl;
        }
    }
    else if (cmd == "help") {
        mostrarMenu();
    }
    else if (cmd == "clear") {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        mostrarBanner();
    }
    else if (cmd == "exit") {
        cout << "\n¡Gracias por usar el sistema! " << endl;
    }
    else if (!cmd.empty()) {
        cout << " Comando desconocido: '" << cmd << "'" << endl;
        cout << "   Escribe 'help' para ver los comandos disponibles" << endl;
    }
}

void ejecutarDemo(FileSystem& fs) {
    cout << "\n=========================================" << endl;
    cout << "|        DEMOSTRACIÓN AUTOMÁTICA        |" << endl;
    cout << "===========================================" << endl;
    
    cout << "\n[1/5] Creando estructura de carpetas..." << endl;
    fs.crearNodo("/documentos", NodeType::FOLDER);
    fs.crearNodo("/imagenes", NodeType::FOLDER);
    fs.crearNodo("/proyectos", NodeType::FOLDER);
    fs.crearNodo("/documentos/trabajo", NodeType::FOLDER);
    fs.crearNodo("/documentos/personal", NodeType::FOLDER);
    cout << "✓ Carpetas creadas" << endl;
    
    cout << "\n[2/5] Creando archivos de ejemplo..." << endl;
    fs.crearNodo("/documentos/readme.txt", NodeType::FILE, "Bienvenido al sistema");
    fs.crearNodo("/documentos/trabajo/informe.doc", NodeType::FILE, "Reporte Q3");
    fs.crearNodo("/imagenes/foto1.jpg", NodeType::FILE, "");
    fs.crearNodo("/proyectos/main.cpp", NodeType::FILE, "#include <iostream>");
    cout << "✓ Archivos creados" << endl;
    
    cout << "\n[3/5] Mostrando estructura:" << endl;
    fs.listarHijos("/");
    
    cout << "\n[4/5] Probando búsqueda por prefijo 'pro':" << endl;
    auto resultados = fs.buscarPorPrefijo("pro");
    for (const auto& res : resultados) {
        cout << "  • " << res << endl;
    }
    
    cout << "\n[5/5] Exportando árbol completo:" << endl;
    auto recorrido = fs.exportarPreorden();
    for (size_t i = 0; i < min(recorrido.size(), size_t(10)); i++) {
        cout << recorrido[i] << endl;
    }
    if (recorrido.size() > 10) {
        cout << "... (mostrando 10 de " << recorrido.size() << " nodos)" << endl;
    }
    
    cout << "\n Demo completada. Sistema listo para usar." << endl;
}

int main() {
    FileSystem fs;
    mostrarBanner();
    
    cout << "\n¿Desea ejecutar la demostración automática? (s/n): ";
    string respuesta;
    getline(cin, respuesta);
    
    if (respuesta == "s" || respuesta == "S") {
        ejecutarDemo(fs);
    }
    
    mostrarMenu();
    
    string comando;
    while (true) {
        cout << "\n📁 > ";
        getline(cin, comando);
        
        if (comando.empty()) {
            continue;
        }
        
        if (comando == "exit") {
            cout << "\n¡Gracias por usar el sistema! 👋" << endl;
            break;
        }
        
        ejecutarComando(fs, comando);
    }
    
    return 0;
}
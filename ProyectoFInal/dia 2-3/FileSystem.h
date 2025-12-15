#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "Node.h"
#include <unordered_map>
#include <sstream>
#include <iostream>

using namespace std;

class FileSystem {
private:
    shared_ptr<Node> root;
    unordered_map<int, shared_ptr<Node>> nodeMap;  // id -> Node
    unordered_map<string, int> nombreMap;  // nombre -> id
    int nextId;
    
    // HELPER: Dividir ruta en partes
    vector<string> splitRuta(const string& ruta) {
        vector<string> partes;
        stringstream ss(ruta);
        string parte;
        
        while (getline(ss, parte, '/')) {
            if (!parte.empty()) {
                partes.push_back(parte);
            }
        }
        return partes;
    }
    
    // HELPER: Navegar a una ruta específica
    shared_ptr<Node> navegarRuta(const string& ruta) {
        if (ruta == "/" || ruta.empty()) {
            return root;
        }
        
        vector<string> partes = splitRuta(ruta);
        auto nodoActual = root;
        
        for (const auto& parte : partes) {
            auto hijo = nodoActual->findChild(parte);
            if (!hijo) {
                return nullptr;  // Ruta no existe
            }
            nodoActual = hijo;
        }
        return nodoActual;
    }
    
public:
    FileSystem() : nextId(1) {
        root = make_shared<Node>(0, "root", NodeType::FOLDER);
        nodeMap[0] = root;
    }
    
    // OPERACIÓN: Crear nodo en una ruta
    bool crearNodo(const string& ruta, NodeType tipo, const string& contenido = "") {
        vector<string> partes = splitRuta(ruta);
        if (partes.empty()) {
            cout << "Error: Ruta inválida" << endl;
            return false;
        }
        
        string nombreNodo = partes.back();
        partes.pop_back();
        
        // Construir ruta del padre
        string rutaPadre = "/";
        if (!partes.empty()) {
            rutaPadre = "/" + partes[0];
            for (size_t i = 1; i < partes.size(); i++) {
                rutaPadre += "/" + partes[i];
            }
        }
        
        auto padre = navegarRuta(rutaPadre);
        if (!padre) {
            cout << "Error: Ruta padre no existe: " << rutaPadre << endl;
            return false;
        }
        
        if (!padre->isFolder()) {
            cout << "Error: El padre debe ser una carpeta" << endl;
            return false;
        }
        
        if (padre->findChild(nombreNodo)) {
            cout << "Error: Ya existe un nodo con ese nombre" << endl;
            return false;
        }
        
        // Crear el nuevo nodo
        auto nuevoNodo = make_shared<Node>(nextId++, nombreNodo, tipo, contenido);
        nuevoNodo->parent = padre.get();
        padre->children.push_back(nuevoNodo);
        
        // Actualizar índices
        nodeMap[nuevoNodo->id] = nuevoNodo;
        nombreMap[nombreNodo] = nuevoNodo->id;
        
        return true;
    }
    
    // OPERACIÓN: Eliminar nodo
    bool eliminarNodo(const string& ruta) {
        auto nodo = navegarRuta(ruta);
        if (!nodo) {
            cout << "Error: Nodo no existe" << endl;
            return false;
        }
        
        if (nodo == root) {
            cout << "Error: No se puede eliminar la raíz" << endl;
            return false;
        }
        
        // Eliminar del padre
        auto padre = nodeMap[nodo->parent->id];
        auto& hijos = padre->children;
        hijos.erase(
            remove_if(hijos.begin(), hijos.end(),
                [&](const shared_ptr<Node>& n) { return n->id == nodo->id; }),
            hijos.end()
        );
        
        // Eliminar de índices
        nombreMap.erase(nodo->nombre);
        
        return true;
    }
    
    // OPERACIÓN: Listar hijos de una carpeta
    void listarHijos(const string& ruta) {
        auto nodo = navegarRuta(ruta);
        if (!nodo) {
            cout << "Error: Nodo no existe" << endl;
            return;
        }
        
        if (!nodo->isFolder()) {
            cout << "Error: El nodo no es una carpeta" << endl;
            return;
        }
        
        cout << "Contenido de " << nodo->getRuta() << ":" << endl;
        for (const auto& hijo : nodo->children) {
            string tipo = hijo->isFolder() ? "[DIR]" : "[FILE]";
            cout << "  " << tipo << " " << hijo->nombre << endl;
        }
    }
    
    // OPERACIÓN: Mostrar información de un nodo
    void mostrarInfo(const string& ruta) {
        auto nodo = navegarRuta(ruta);
        if (!nodo) {
            cout << "Error: Nodo no existe" << endl;
            return;
        }
        
        cout << "=== INFORMACIÓN DEL NODO ===" << endl;
        cout << "Ruta: " << nodo->getRuta() << endl;
        cout << "Tipo: " << (nodo->isFolder() ? "Carpeta" : "Archivo") << endl;
        cout << "ID: " << nodo->id << endl;
        if (nodo->isFile()) {
            cout << "Contenido: " << nodo->contenido << endl;
        }
        cout << "Altura del subárbol: " << nodo->calcularAltura() << endl;
        cout << "Tamaño del subárbol: " << nodo->calcularTamano() << " nodos" << endl;
    }
    
    shared_ptr<Node> getRoot() {
        return root;
    }
};

#endif
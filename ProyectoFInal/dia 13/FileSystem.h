#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "Node.h"
#include "Trie.h"
#include <unordered_map>
#include <queue>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

class FileSystem {
private:
    shared_ptr<Node> root;
    unordered_map<int, shared_ptr<Node>> nodeMap;
    unordered_map<string, vector<int>> nombreMap;  // Cambiado para soportar múltiples nodos con mismo nombre
    Trie trie;
    vector<shared_ptr<Node>> papelera;
    int nextId;
    
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
    
    shared_ptr<Node> navegarRuta(const string& ruta) {
        if (ruta == "/" || ruta.empty()) {
            return root;
        }
        
        vector<string> partes = splitRuta(ruta);
        auto nodoActual = root;
        
        for (const auto& parte : partes) {
            auto hijo = nodoActual->findChild(parte);
            if (!hijo) {
                return nullptr;
            }
            nodoActual = hijo;
        }
        return nodoActual;
    }
    
    void recorridoPreordenRecursivo(shared_ptr<Node> nodo, vector<string>& resultado, int nivel) {
        string indent(nivel * 2, ' ');
        string tipo = nodo->isFolder() ? "[DIR]" : "[FILE]";
        resultado.push_back(indent + tipo + " " + nodo->nombre);
        
        for (auto& hijo : nodo->children) {
            recorridoPreordenRecursivo(hijo, resultado, nivel + 1);
        }
    }
    
    void eliminarDeMapaRecursivo(shared_ptr<Node> nodo) {
        // Eliminar hijos recursivamente
        for (auto& hijo : nodo->children) {
            eliminarDeMapaRecursivo(hijo);
        }
        
        // Eliminar el nodo actual del nodeMap
        nodeMap.erase(nodo->id);
        
        // Eliminar del nombreMap
        auto it = nombreMap.find(nodo->nombre);
        if (it != nombreMap.end()) {
            auto& ids = it->second;
            ids.erase(remove(ids.begin(), ids.end(), nodo->id), ids.end());
            if (ids.empty()) {
                nombreMap.erase(nodo->nombre);
            }
        }
    }
    
public:
    FileSystem() : nextId(1) {
        root = make_shared<Node>(0, "root", NodeType::FOLDER);
        nodeMap[0] = root;
        nombreMap["root"].push_back(0);
    }
    
    bool crearNodo(const string& ruta, NodeType tipo, const string& contenido = "") {
        vector<string> partes = splitRuta(ruta);
        if (partes.empty()) {
            cout << "Error: Ruta invalida" << endl;
            return false;
        }
        
        string nombreNodo = partes.back();
        partes.pop_back();
        
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
        
        auto nuevoNodo = make_shared<Node>(nextId++, nombreNodo, tipo, contenido);
        nuevoNodo->parent = padre.get();
        padre->children.push_back(nuevoNodo);
        
        nodeMap[nuevoNodo->id] = nuevoNodo;
        nombreMap[nombreNodo].push_back(nuevoNodo->id);
        trie.insertar(nombreNodo);
        
        return true;
    }
    
    bool moverNodo(const string& rutaOrigen, const string& rutaDestino) {
        auto nodoOrigen = navegarRuta(rutaOrigen);
        if (!nodoOrigen) {
            cout << "Error: Nodo origen no existe" << endl;
            return false;
        }
        
        if (nodoOrigen == root) {
            cout << "Error: No se puede mover la raiz" << endl;
            return false;
        }
        
        auto nodoDestino = navegarRuta(rutaDestino);
        if (!nodoDestino) {
            cout << "Error: Nodo destino no existe" << endl;
            return false;
        }
        
        if (!nodoDestino->isFolder()) {
            cout << "Error: El destino debe ser una carpeta" << endl;
            return false;
        }
        
        if (nodoDestino->findChild(nodoOrigen->nombre)) {
            cout << "Error: Ya existe un nodo con ese nombre en el destino" << endl;
            return false;
        }
        
        // Verificar que no estemos moviendo una carpeta dentro de si misma
        auto temp = nodoDestino;
        while (temp && temp->parent) {
            if (temp == nodoOrigen) {
                cout << "Error: No se puede mover una carpeta dentro de si misma" << endl;
                return false;
            }
            auto it = nodeMap.find(temp->parent->id);
            if (it == nodeMap.end()) break;
            temp = it->second;
        }
        
        // Validar que el nodo origen tiene padre
        if (!nodoOrigen->parent) {
            cout << "Error: El nodo origen no tiene padre" << endl;
            return false;
        }
        
        // Eliminar del padre original
        auto itPadre = nodeMap.find(nodoOrigen->parent->id);
        if (itPadre == nodeMap.end()) {
            cout << "Error: Padre original no encontrado en nodeMap" << endl;
            return false;
        }
        
        auto padreOriginal = itPadre->second;
        auto& hijos = padreOriginal->children;
        hijos.erase(remove_if(hijos.begin(), hijos.end(),
            [&](const shared_ptr<Node>& n) { return n->id == nodoOrigen->id; }), hijos.end());
        
        // Agregar al nuevo padre
        nodoOrigen->parent = nodoDestino.get();
        nodoDestino->children.push_back(nodoOrigen);
        
        return true;
    }
    
    bool renombrarNodo(const string& ruta, const string& nuevoNombre) {
        auto nodo = navegarRuta(ruta);
        if (!nodo) {
            cout << "Error: Nodo no existe" << endl;
            return false;
        }
        
        if (nodo == root) {
            cout << "Error: No se puede renombrar la raiz" << endl;
            return false;
        }
        
        if (!nodo->parent) {
            cout << "Error: Nodo sin padre" << endl;
            return false;
        }
        
        auto itPadre = nodeMap.find(nodo->parent->id);
        if (itPadre == nodeMap.end()) {
            cout << "Error: Padre no encontrado" << endl;
            return false;
        }
        
        auto padre = itPadre->second;
        if (padre->findChild(nuevoNombre)) {
            cout << "Error: Ya existe un nodo con ese nombre" << endl;
            return false;
        }
        
        // Eliminar nombre antiguo del trie y nombreMap
        trie.eliminar(nodo->nombre);
        auto it = nombreMap.find(nodo->nombre);
        if (it != nombreMap.end()) {
            auto& ids = it->second;
            ids.erase(remove(ids.begin(), ids.end(), nodo->id), ids.end());
            if (ids.empty()) {
                nombreMap.erase(nodo->nombre);
            }
        }
        
        // Actualizar nombre
        nodo->nombre = nuevoNombre;
        
        // Agregar nuevo nombre
        nombreMap[nuevoNombre].push_back(nodo->id);
        trie.insertar(nuevoNombre);
        
        return true;
    }
    
    bool eliminarNodo(const string& ruta) {
        auto nodo = navegarRuta(ruta);
        if (!nodo) {
            cout << "Error: Nodo no existe" << endl;
            return false;
        }
        
        if (nodo == root) {
            cout << "Error: No se puede eliminar la raiz" << endl;
            return false;
        }
        
        // Validar que el nodo tiene padre
        if (!nodo->parent) {
            cout << "Error: Nodo sin padre" << endl;
            return false;
        }
        
        // Buscar padre en nodeMap
        auto itPadre = nodeMap.find(nodo->parent->id);
        if (itPadre == nodeMap.end()) {
            cout << "Error: Padre no encontrado en nodeMap" << endl;
            return false;
        }
        
        // Mover a papelera
        papelera.push_back(nodo);
        
        // Eliminar del padre
        auto padre = itPadre->second;
        auto& hijos = padre->children;
        hijos.erase(remove_if(hijos.begin(), hijos.end(),
            [&](const shared_ptr<Node>& n) { return n->id == nodo->id; }), hijos.end());
        
        // Eliminar recursivamente de los mapas
        eliminarDeMapaRecursivo(nodo);
        
        // Eliminar del trie
        trie.eliminar(nodo->nombre);
        
        return true;
    }
    
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
        if (nodo->children.empty()) {
            cout << "  (vacio)" << endl;
            return;
        }
        
        for (const auto& hijo : nodo->children) {
            string tipo = hijo->isFolder() ? "[DIR]" : "[FILE]";
            cout << "  " << tipo << " " << hijo->nombre;
            if (hijo->isFile() && !hijo->contenido.empty()) {
                cout << " (tamaño: " << hijo->contenido.length() << " bytes)";
            }
            cout << endl;
        }
    }
    
    void mostrarRuta(const string& ruta) {
        auto nodo = navegarRuta(ruta);
        if (!nodo) {
            cout << "Error: Nodo no existe" << endl;
            return;
        }
        
        cout << "Ruta completa: " << nodo->getRuta() << endl;
        cout << "Tipo: " << (nodo->isFolder() ? "Carpeta" : "Archivo") << endl;
        cout << "ID: " << nodo->id << endl;
        if (nodo->isFile()) {
            cout << "Contenido: " << nodo->contenido << endl;
        }
        cout << "Altura del subarbol: " << nodo->calcularAltura() << endl;
        cout << "Tamaño del subarbol: " << nodo->calcularTamano() << " nodos" << endl;
    }
    
    vector<string> exportarPreorden() {
        vector<string> resultado;
        recorridoPreordenRecursivo(root, resultado, 0);
        return resultado;
    }
    
    vector<string> buscarPorPrefijo(const string& prefijo) {
        return trie.autocompletar(prefijo);
    }
    
    bool buscarExacto(const string& nombre) {
        auto it = nombreMap.find(nombre);
        if (it == nombreMap.end() || it->second.empty()) {
            return false;
        }
        
        // Verificar que al menos un nodo con ese nombre existe en nodeMap
        for (int id : it->second) {
            if (nodeMap.find(id) != nodeMap.end()) {
                return true;
            }
        }
        return false;
    }
    
    vector<shared_ptr<Node>> buscarTodosExactos(const string& nombre) {
        vector<shared_ptr<Node>> resultados;
        auto it = nombreMap.find(nombre);
        if (it == nombreMap.end()) {
            return resultados;
        }
        
        for (int id : it->second) {
            auto itNode = nodeMap.find(id);
            if (itNode != nodeMap.end()) {
                resultados.push_back(itNode->second);
            }
        }
        return resultados;
    }
    
    void mostrarPapelera() {
        cout << "=== PAPELERA ===" << endl;
        if (papelera.empty()) {
            cout << "La papelera esta vacia" << endl;
            return;
        }
        
        for (size_t i = 0; i < papelera.size(); i++) {
            cout << i << ". " << papelera[i]->nombre 
                 << " [" << (papelera[i]->isFolder() ? "DIR" : "FILE") << "]"
                 << " (ID: " << papelera[i]->id << ")" << endl;
        }
    }
    
    bool restaurarDePapelera(size_t indice, const string& rutaDestino) {
        if (indice >= papelera.size()) {
            cout << "Error: Indice invalido" << endl;
            return false;
        }
        
        auto nodo = papelera[indice];
        auto destino = navegarRuta(rutaDestino);
        
        if (!destino) {
            cout << "Error: Ruta destino no existe" << endl;
            return false;
        }
        
        if (!destino->isFolder()) {
            cout << "Error: El destino debe ser una carpeta" << endl;
            return false;
        }
        
        if (destino->findChild(nodo->nombre)) {
            cout << "Error: Ya existe un nodo con ese nombre en el destino" << endl;
            return false;
        }
        
        // Restaurar el nodo
        nodo->parent = destino.get();
        destino->children.push_back(nodo);
        
        // Agregar de vuelta a los mapas
        nodeMap[nodo->id] = nodo;
        nombreMap[nodo->nombre].push_back(nodo->id);
        trie.insertar(nodo->nombre);
        
        // Eliminar de la papelera
        papelera.erase(papelera.begin() + indice);
        
        cout << "Nodo restaurado exitosamente" << endl;
        return true;
    }
    
    void vaciarPapelera() {
        papelera.clear();
        cout << "Papelera vaciada" << endl;
    }
    
    shared_ptr<Node> getRoot() {
        return root;
    }
    
    int getTotalNodos() {
        return nodeMap.size();
    }
    
    void mostrarEstadisticas() {
        cout << "=== ESTADISTICAS DEL SISTEMA ===" << endl;
    cout << "Total de nodos: " << nodeMap.size() << endl;
    cout << "Altura del arbol: " << root->calcularAltura() << endl;
    cout << "Nodos en papelera: " << papelera.size() << endl;
    cout << "Nombres unicos: " << nombreMap.size() << endl;
    
    // Contar total de archivos y carpetas
    int totalArchivos = 0;
    int totalCarpetas = 0;
    for (const auto& par : nodeMap) {
        if (par.second->isFile()) {
            totalArchivos++;
        } else {
            totalCarpetas++;
        }
    }
    cout << "Total de archivos: " << totalArchivos << endl;
    cout << "Total de carpetas: " << totalCarpetas << endl;
    }
};

#endif
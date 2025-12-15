#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "Node.h"
#include "Trie.h"
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <queue>

using namespace std;

class FileSystem {
private:
    shared_ptr<Node> root;
    unordered_map<int, shared_ptr<Node>> nodeMap;
    unordered_map<string, int> nombreMap;
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
    
public:
    FileSystem() : nextId(1) {
        root = make_shared<Node>(0, "root", NodeType::FOLDER);
        nodeMap[0] = root;
    }
    
    
    bool crearNodo(const string& ruta, NodeType tipo, const string& contenido = "") {
        vector<string> partes = splitRuta(ruta);
        if (partes.empty()) {
            cout << "Error: Ruta inválida" << endl;
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
        nombreMap[nombreNodo] = nuevoNodo->id;
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
            cout << "Error: No se puede mover la raíz" << endl;
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
        
        
        auto temp = nodoDestino;
        while (temp) {
            if (temp == nodoOrigen) {
                cout << "Error: No se puede mover una carpeta dentro de sí misma" << endl;
                return false;
            }
            temp = (temp->parent ? nodeMap[temp->parent->id] : nullptr);
        }
        
       
        auto padreOriginal = nodeMap[nodoOrigen->parent->id];
        auto& hijos = padreOriginal->children;
        hijos.erase(remove_if(hijos.begin(), hijos.end(),
            [&](const shared_ptr<Node>& n) { return n->id == nodoOrigen->id; }), hijos.end());
        
        
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
            cout << "Error: No se puede renombrar la raíz" << endl;
            return false;
        }
        
        auto padre = nodeMap[nodo->parent->id];
        if (padre->findChild(nuevoNombre)) {
            cout << "Error: Ya existe un nodo con ese nombre" << endl;
            return false;
        }
        
        
        trie.eliminar(nodo->nombre);
        nombreMap.erase(nodo->nombre);
        
        nodo->nombre = nuevoNombre;
        
        nombreMap[nuevoNombre] = nodo->id;
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
            cout << "Error: No se puede eliminar la raíz" << endl;
            return false;
        }
        
        // NUEVO: Mover a papelera
        papelera.push_back(nodo);
        
        auto padre = nodeMap[nodo->parent->id];
        auto& hijos = padre->children;
        hijos.erase(remove_if(hijos.begin(), hijos.end(),
            [&](const shared_ptr<Node>& n) { return n->id == nodo->id; }), hijos.end());
        
        trie.eliminar(nodo->nombre);
        nombreMap.erase(nodo->nombre);
        
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
        cout << "Altura del subárbol: " << nodo->calcularAltura() << endl;
        cout << "Tamaño del subárbol: " << nodo->calcularTamano() << " nodos" << endl;
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
        return nombreMap.find(nombre) != nombreMap.end();
    }
    
    void mostrarPapelera() {
        cout << "=== PAPELERA ===" << endl;
        if (papelera.empty()) {
            cout << "La papelera está vacía" << endl;
            return;
        }
        
        for (size_t i = 0; i < papelera.size(); i++) {
            cout << i << ". " << papelera[i]->nombre 
                 << " [" << (papelera[i]->isFolder() ? "DIR" : "FILE") << "]" << endl;
        }
    }
    
   
    void vaciarPapelera() {
        papelera.clear();
        cout << "Papelera vaciada" << endl;
    }
    
    shared_ptr<Node> getRoot() {
        return root;
    }
};

#endif
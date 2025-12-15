#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;

enum class NodeType {
    FOLDER,
    FILE
};

class Node {
public:
    int id;
    string nombre;
    NodeType tipo;
    string contenido;
    vector<shared_ptr<Node>> children;  // CAMBIO: Ahora usa smart pointers
    Node* parent;
    
    // Constructor
    Node(int _id, string _nombre, NodeType _tipo, string _contenido = "")
        : id(_id), nombre(_nombre), tipo(_tipo), contenido(_contenido), parent(nullptr) {}
    
    // Métodos de verificación de tipo
    bool isFolder() const {
        return tipo == NodeType::FOLDER;
    }
    
    bool isFile() const {
        return tipo == NodeType::FILE;
    }
    
    // NUEVO: Obtener ruta completa desde la raíz
    string getRuta() const {
        if (parent == nullptr) {
            return "/" + nombre;
        }
        string rutaPadre = parent->getRuta();
        if (rutaPadre == "/") {
            return "/" + nombre;
        }
        return rutaPadre + "/" + nombre;
    }
    
    // NUEVO: Buscar un hijo por nombre
    shared_ptr<Node> findChild(const string& nombreHijo) {
        for (auto& hijo : children) {
            if (hijo->nombre == nombreHijo) {
                return hijo;
            }
        }
        return nullptr;
    }
    
    // NUEVO: Calcular altura del subárbol
    int calcularAltura() {
        if (children.empty()) {
            return 1;  // Nodo hoja tiene altura 1
        }
        int alturaMax = 0;
        for (auto& hijo : children) {
            alturaMax = max(alturaMax, hijo->calcularAltura());
        }
        return alturaMax + 1;
    }
    
    // NUEVO: Calcular tamaño del subárbol (número de nodos)
    int calcularTamano() {
        int tamano = 1;  // Contar el nodo actual
        for (auto& hijo : children) {
            tamano += hijo->calcularTamano();
        }
        return tamano;
    }
    
    // NUEVO: Verificar si es una ruta válida
    bool esRutaValida() const {
        // No permitir nombres vacíos
        if (nombre.empty()) return false;
        
        // No permitir caracteres especiales en nombres
        for (char c : nombre) {
            if (c == '/' || c == '\\') {
                return false;
            }
        }
        return true;
    }
};

#endif
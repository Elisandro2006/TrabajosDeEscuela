#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <memory>

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
    vector<shared_ptr<Node>> children;
    Node* parent;
    
    Node(int _id, string _nombre, NodeType _tipo, string _contenido = "")
        : id(_id), nombre(_nombre), tipo(_tipo), contenido(_contenido), parent(nullptr) {}
    
    bool isFolder() const {
        return tipo == NodeType::FOLDER;
    }
    
    bool isFile() const {
        return tipo == NodeType::FILE;
    }
    
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
    
    shared_ptr<Node> findChild(const string& nombreHijo) {
        for (auto& hijo : children) {
            if (hijo->nombre == nombreHijo) {
                return hijo;
            }
        }
        return nullptr;
    }
    
    int calcularAltura() {
        if (children.empty()) {
            return 1;
        }
        int alturaMax = 0;
        for (auto& hijo : children) {
            alturaMax = max(alturaMax, hijo->calcularAltura());
        }
        return alturaMax + 1;
    }
    
    int calcularTamano() {
        int tamano = 1;
        for (auto& hijo : children) {
            tamano += hijo->calcularTamano();
        }
        return tamano;
    }
};

#endif
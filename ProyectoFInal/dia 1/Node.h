#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

using namespace std;

// Enum para definir el tipo de nodo
enum class NodeType {
    FOLDER,
    FILE
};

// Clase Node - Representa un nodo en el árbol de archivos
class Node {
public:
    // Atributos básicos
    int id;
    string nombre;
    NodeType tipo;
    string contenido;  // Solo para archivos
    vector<Node*> children;  // Lista de hijos
    Node* parent;  // Referencia al padre
    
    // Constructor
    Node(int _id, string _nombre, NodeType _tipo, string _contenido = "")
        : id(_id), nombre(_nombre), tipo(_tipo), contenido(_contenido), parent(nullptr) {}
    
    // Métodos básicos para verificar tipo
    bool isFolder() const {
        return tipo == NodeType::FOLDER;
    }
    
    bool isFile() const {
        return tipo == NodeType::FILE;
    }
};

#endif
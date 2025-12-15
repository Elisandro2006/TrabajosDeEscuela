#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

// Nodo del Trie
class TrieNode {
public:
    unordered_map<char, shared_ptr<TrieNode>> children;
    bool isEndOfWord;
    
    TrieNode() : isEndOfWord(false) {}
};


class Trie {
private:
    shared_ptr<TrieNode> root;
    
    
    void buscarConPrefijo(shared_ptr<TrieNode> nodo, string prefijo, vector<string>& resultados) {
        if (nodo->isEndOfWord) {
            resultados.push_back(prefijo);
        }
        
        
        for (auto& par : nodo->children) {
            buscarConPrefijo(par.second, prefijo + par.first, resultados);
        }
    }
    
public:
    Trie() {
        root = make_shared<TrieNode>();
    }
    
    
    void insertar(const string& palabra) {
        if (palabra.empty()) return;
        
        auto nodo = root;
        for (char c : palabra) {
            
            if (nodo->children.find(c) == nodo->children.end()) {
                nodo->children[c] = make_shared<TrieNode>();
            }
            nodo = nodo->children[c];
        }
        nodo->isEndOfWord = true;
    }
    
    
    void eliminar(const string& palabra) {
        eliminarRecursivo(root, palabra, 0);
    }
    
    bool eliminarRecursivo(shared_ptr<TrieNode> nodo, const string& palabra, int indice) {
        if (indice == palabra.length()) {
            
            if (!nodo->isEndOfWord) {
                return false;  
            }
            nodo->isEndOfWord = false;
            
            return nodo->children.empty();
        }
        
        char c = palabra[indice];
        if (nodo->children.find(c) == nodo->children.end()) {
            return false;  
        }
        
        auto siguienteNodo = nodo->children[c];
        bool debeEliminarNodoActual = eliminarRecursivo(siguienteNodo, palabra, indice + 1);
        
        if (debeEliminarNodoActual) {
            nodo->children.erase(c);
            
            return nodo->children.empty() && !nodo->isEndOfWord;
        }
        
        return false;
    }
    
    
    bool buscar(const string& palabra) {
        auto nodo = root;
        for (char c : palabra) {
            if (nodo->children.find(c) == nodo->children.end()) {
                return false;
            }
            nodo = nodo->children[c];
        }
        return nodo->isEndOfWord;
    }
    
    
    vector<string> autocompletar(const string& prefijo) {
        vector<string> resultados;
        auto nodo = root;
        
        
        for (char c : prefijo) {
            if (nodo->children.find(c) == nodo->children.end()) {
                return resultados;  
            }
            nodo = nodo->children[c];
        }
        
       
        buscarConPrefijo(nodo, prefijo, resultados);
        return resultados;
    }
    
    
    bool existePrefijo(const string& prefijo) {
        auto nodo = root;
        for (char c : prefijo) {
            if (nodo->children.find(c) == nodo->children.end()) {
                return false;
            }
            nodo = nodo->children[c];
        }
        return true;
    }
    
    
    int contarConPrefijo(const string& prefijo) {
        return autocompletar(prefijo).size();
    }
};

#endif
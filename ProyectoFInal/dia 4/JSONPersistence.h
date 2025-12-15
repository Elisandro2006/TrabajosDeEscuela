#ifndef JSON_PERSISTENCE_H
#define JSON_PERSISTENCE_H

#include "Node.h"
#include <fstream>
#include <sstream>

using namespace std;

class JSONPersistence {
public:
    // Serializar un nodo a formato JSON (recursivo)
    static string nodoToJSON(shared_ptr<Node> nodo, int nivel = 0) {
        string indent(nivel * 2, ' ');
        string indent2((nivel + 1) * 2, ' ');
        
        stringstream json;
        json << indent << "{" << endl;
        json << indent2 << "\"id\": " << nodo->id << "," << endl;
        json << indent2 << "\"nombre\": \"" << nodo->nombre << "\"," << endl;
        json << indent2 << "\"tipo\": \"" << (nodo->isFolder() ? "carpeta" : "archivo") << "\"," << endl;
        json << indent2 << "\"contenido\": \"" << escaparJSON(nodo->contenido) << "\"," << endl;
        json << indent2 << "\"children\": [" << endl;
        
        for (size_t i = 0; i < nodo->children.size(); i++) {
            json << nodoToJSON(nodo->children[i], nivel + 2);
            if (i < nodo->children.size() - 1) {
                json << ",";
            }
            json << endl;
        }
        
        json << indent2 << "]" << endl;
        json << indent << "}";
        
        return json.str();
    }
    
    // Guardar árbol completo en archivo JSON
    static bool guardarJSON(shared_ptr<Node> root, const string& nombreArchivo) {
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cerr << "Error: No se pudo abrir el archivo para escribir: " << nombreArchivo << endl;
            return false;
        }
        
        archivo << nodoToJSON(root) << endl;
        archivo.close();
        
        cout << "✓ Árbol guardado en: " << nombreArchivo << endl;
        return true;
    }
    
    // Cargar árbol desde archivo JSON (simplificado)
    // NOTA: Implementación completa requiere parser JSON completo
    static shared_ptr<Node> cargarJSON(const string& nombreArchivo, int& nextId) {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cerr << "Error: No se pudo abrir el archivo: " << nombreArchivo << endl;
            return nullptr;
        }
        
        cout << "✓ Cargando desde: " << nombreArchivo << endl;
        cout << "NOTA: Implementación básica - para producción usar librería JSON completa" << endl;
        
        // Por simplicidad, retornamos nullptr
        // En producción usar nlohmann/json o similar
        archivo.close();
        return nullptr;
    }

private:
    // Escapar caracteres especiales en JSON
    static string escaparJSON(const string& str) {
        string resultado;
        for (char c : str) {
            switch (c) {
                case '"':  resultado += "\\\""; break;
                case '\\': resultado += "\\\\"; break;
                case '\n': resultado += "\\n"; break;
                case '\r': resultado += "\\r"; break;
                case '\t': resultado += "\\t"; break;
                default:   resultado += c; break;
            }
        }
        return resultado;
    }
};
#include <iostream>
#include <string> // necesario para std::string

struct Persona
{
    std::string nombre;
    int edad;
    float estatura;    
};

int main()
{
    Persona personas[3];
    
    for(int i=0; i<3; i++)
    {
        std::cout << "Persona " << i+1 << "\n";
        
        std::cout << "Ingrese el nombre: ";
        std::cin.ignore(); // limpiar salto de línea pendiente en el buffer
        std::getline(std::cin, personas[i].nombre);
        
        std::cout << "Ingrese la edad: ";
        std::cin >> personas[i].edad;
        
        std::cout << "Ingrese la estatura (m): ";
        std::cin >> personas[i].estatura;
        std::cout << "\n";
    }

    // Mostrar resultados
    std::cout << "\n--- Lista de personas ---\n";
    for(int i=0; i<3; i++)
    {
        std::cout << "Persona " << i+1 << ":\n";
        std::cout << "Nombre: " << personas[i].nombre << "\n";
        std::cout << "Edad: " << personas[i].edad << "\n";
        std::cout << "Estatura: " << personas[i].estatura << " m\n\n";
    }

    return 0;
}

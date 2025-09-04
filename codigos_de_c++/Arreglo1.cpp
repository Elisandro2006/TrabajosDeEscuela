#include <iostream>

int main() {
    std::string nombre[5] = {"Elisandro", "Clarissa", "Carlos", "Marta", "Sofia"};
    for (int i = 0; i < 5; i++) {
        std::cout << nombre[i] << std::endl;
    }
    return 0;  
}
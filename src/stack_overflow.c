/*
 * Exemplo de Stack Overflow
 * 
 * Este programa demonstra como causar um stack overflow através de
 * recursão infinita sem condição de parada.
 */

#include <stdio.h>
#include <stdlib.h>

// Função recursiva que causa stack overflow
void recursive_function(int depth) {
    char buffer[1024]; // Aloca memória no stack a cada chamada
    
    printf("Profundidade da recursão: %d\n", depth);
    
    // Recursão sem condição de parada - causará stack overflow
    recursive_function(depth + 1);
}

int main() {
    printf("=== DEMONSTRAÇÃO: STACK OVERFLOW ===\n");
    printf("Iniciando recursão infinita que causará stack overflow...\n");
    
    // Inicia a recursão que causará o erro
    recursive_function(1);
    
    // Esta linha nunca será executada
    printf("Esta linha nunca será impressa.\n");
    
    return 0;
}

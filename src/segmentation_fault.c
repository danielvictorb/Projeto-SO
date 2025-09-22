/*
 * Exemplo de Segmentation Fault
 * 
 * Este programa demonstra várias formas de causar segmentation fault,
 * incluindo acesso a ponteiros nulos e áreas de memória inválidas.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void null_pointer_access() {
    printf("Testando acesso a ponteiro nulo...\n");
    
    int *ptr = NULL;
    // Tentativa de escrever em um ponteiro nulo - causa SIGSEGV
    *ptr = 42;
    
    printf("Esta linha não será executada.\n");
}

void invalid_memory_access() {
    printf("Testando acesso a memória inválida...\n");
    
    // Tentativa de acessar uma área de memória que não pertence ao processo
    int *ptr = (int*)0x12345678;
    *ptr = 100;
    
    printf("Esta linha não será executada.\n");
}

void array_bounds_violation() {
    printf("Testando violação de bounds de array...\n");
    
    int arr[10];
    // Acesso muito além dos limites do array
    arr[10000] = 42;
    
    printf("Esta linha não será executada.\n");
}

int main(int argc, char *argv[]) {
    printf("=== DEMONSTRAÇÃO: SEGMENTATION FAULT ===\n");
    
    int option = 1;
    if (argc > 1) {
        option = atoi(argv[1]);
    }
    
    switch(option) {
        case 1:
            null_pointer_access();
            break;
        case 2:
            invalid_memory_access();
            break;
        case 3:
            array_bounds_violation();
            break;
        default:
            printf("Opções: 1=ponteiro nulo, 2=memória inválida, 3=array bounds\n");
            null_pointer_access();
    }
    
    return 0;
}

/*
 * Exemplo de Memory Leak
 * 
 * Este programa demonstra diferentes tipos de vazamentos de memória,
 * desde vazamentos simples até vazamentos mais complexos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void simple_memory_leak() {
    printf("Demonstrando vazamento simples de memória...\n");
    
    for (int i = 0; i < 1000; i++) {
        // Aloca memória mas nunca libera
        char *leaked_memory = (char*)malloc(1024);
        if (leaked_memory) {
            sprintf(leaked_memory, "Vazamento %d", i);
            // free(leaked_memory); // Esta linha está comentada - causa vazamento!
        }
        
        // Simula algum trabalho
        usleep(1000); // 1ms
    }
    
    printf("Vazamentos simples criados.\n");
}

void recursive_memory_leak(int depth) {
    if (depth <= 0) return;
    
    // Aloca memória a cada chamada recursiva
    char *buffer = (char*)malloc(2048);
    sprintf(buffer, "Recursão nível %d", depth);
    
    printf("Alocando na profundidade %d\n", depth);
    
    // Chama recursivamente mas nunca libera a memória
    recursive_memory_leak(depth - 1);
    
    // free(buffer); // Nunca libera - causa vazamento!
}

void growing_memory_leak() {
    printf("Demonstrando vazamento crescente...\n");
    
    for (int i = 1; i <= 50; i++) {
        // Aloca blocos cada vez maiores
        size_t size = i * 1024; // 1KB, 2KB, 3KB, ...
        void *ptr = malloc(size);
        
        if (ptr) {
            memset(ptr, i, size); // Preenche com dados
            printf("Alocados %zu bytes (total vazado até agora)\n", size);
        }
        
        // Não libera a memória - vazamento crescente!
        usleep(10000); // 10ms para observar o crescimento
    }
}

int main(int argc, char *argv[]) {
    printf("=== DEMONSTRAÇÃO: MEMORY LEAK ===\n");
    printf("Use 'valgrind' ou 'top' para monitorar o uso de memória\n\n");
    
    int option = 1;
    if (argc > 1) {
        option = atoi(argv[1]);
    }
    
    switch(option) {
        case 1:
            simple_memory_leak();
            break;
        case 2:
            printf("Iniciando vazamento recursivo...\n");
            recursive_memory_leak(20);
            break;
        case 3:
            growing_memory_leak();
            break;
        default:
            printf("Opções: 1=vazamento simples, 2=vazamento recursivo, 3=vazamento crescente\n");
            simple_memory_leak();
    }
    
    printf("\nProcesso ainda em execução com vazamentos ativos...\n");
    printf("Pressione Ctrl+C para terminar\n");
    
    // Mantém o processo vivo para observar os vazamentos
    while(1) {
        sleep(1);
    }
    
    return 0;
}

/*
 * Exemplo de Buffer Overflow
 * 
 * Este programa demonstra como causar buffer overflow através de
 * escrita além dos limites de buffers de diferentes tipos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stack_buffer_overflow() {
    printf("Testando buffer overflow no stack...\n");
    
    char buffer[10];
    // Copia string muito grande para buffer pequeno
    // Isso pode sobrescrever variáveis adjacentes e até mesmo o endereço de retorno
    strcpy(buffer, "Esta string é muito maior que o buffer de 10 caracteres e causará overflow");
    
    printf("Buffer (pode estar corrompido): %s\n", buffer);
}

void heap_buffer_overflow() {
    printf("Testando buffer overflow no heap...\n");
    
    char *buffer = (char*)malloc(10);
    if (!buffer) {
        printf("Erro ao alocar memória\n");
        return;
    }
    
    // Escrita além dos limites do buffer alocado no heap
    strcpy(buffer, "String muito longa para o buffer de 10 bytes alocado no heap");
    
    printf("Buffer heap: %s\n", buffer);
    
    free(buffer);
}

void format_string_vulnerability() {
    printf("Testando vulnerabilidade de format string...\n");
    
    char user_input[] = "%s %s %s %s %s %s %s %s";
    
    // Usar input do usuário diretamente como format string é perigoso
    // Pode causar leitura de memória não autorizada
    printf(user_input);
    printf("\n");
}

int main(int argc, char *argv[]) {
    printf("=== DEMONSTRAÇÃO: BUFFER OVERFLOW ===\n");
    
    int option = 1;
    if (argc > 1) {
        option = atoi(argv[1]);
    }
    
    switch(option) {
        case 1:
            stack_buffer_overflow();
            break;
        case 2:
            heap_buffer_overflow();
            break;
        case 3:
            format_string_vulnerability();
            break;
        default:
            printf("Opções: 1=stack overflow, 2=heap overflow, 3=format string\n");
            stack_buffer_overflow();
    }
    
    return 0;
}

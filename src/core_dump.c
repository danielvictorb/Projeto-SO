/*
 * Exemplo de Core Dump e Outros Erros
 * 
 * Este programa demonstra diferentes situações que causam core dump
 * e outros tipos de erros de execução.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>
#include <string.h>
#include <math.h>

void enable_core_dumps() {
    struct rlimit core_limit;
    core_limit.rlim_cur = RLIM_INFINITY;
    core_limit.rlim_max = RLIM_INFINITY;
    
    if (setrlimit(RLIMIT_CORE, &core_limit) == 0) {
        printf("Core dumps habilitados\n");
    } else {
        printf("Falha ao habilitar core dumps\n");
    }
}

void signal_handler(int sig) {
    printf("Sinal recebido: %d\n", sig);
    exit(sig);
}

void cause_sigsegv() {
    printf("=== CAUSANDO SIGSEGV (Core Dump) ===\n");
    
    // Registra handler para capturar o sinal
    signal(SIGSEGV, signal_handler);
    
    // Acesso a ponteiro nulo - causa SIGSEGV
    int *null_ptr = NULL;
    printf("Tentando acessar ponteiro nulo...\n");
    *null_ptr = 42; // SIGSEGV aqui!
}

void cause_sigfpe() {
    printf("=== CAUSANDO SIGFPE (Floating Point Exception) ===\n");
    
    signal(SIGFPE, signal_handler);
    
    int a = 10;
    int b = 0;
    
    printf("Tentando divisão por zero: %d / %d\n", a, b);
    int result = a / b; // SIGFPE aqui!
    
    printf("Resultado: %d\n", result);
}

void cause_sigill() {
    printf("=== CAUSANDO SIGILL (Illegal Instruction) ===\n");
    
    signal(SIGILL, signal_handler);
    
    // Executa instrução inválida (específica da arquitetura)
    // Em x86/x64, podemos tentar executar código inválido
    printf("Tentando executar instrução ilegal...\n");
    
    // Cria um ponteiro para função com código inválido
    void (*illegal_func)() = (void(*)())"\xFF\xFF\xFF\xFF";
    illegal_func(); // SIGILL aqui!
}

void cause_sigabrt() {
    printf("=== CAUSANDO SIGABRT (Abort) ===\n");
    
    signal(SIGABRT, signal_handler);
    
    printf("Chamando abort()...\n");
    abort(); // SIGABRT aqui!
}

// Função recursiva para esgotar o stack
void recursive_bomb() {
    char huge_buffer[10000];
    memset(huge_buffer, 'A', sizeof(huge_buffer));
    printf("Stack depth aumentando...\n");
    recursive_bomb(); // Recursão infinita
}

void cause_stackoverflow_signal() {
    printf("=== CAUSANDO SIGSEGV por Stack Overflow ===\n");
    
    signal(SIGSEGV, signal_handler);
    
    recursive_bomb();
}

void cause_bus_error() {
    printf("=== CAUSANDO SIGBUS (Bus Error) ===\n");
    
    signal(SIGBUS, signal_handler);
    
    // Tenta acessar memória com alinhamento incorreto
    // Isso pode causar SIGBUS em algumas arquiteturas
    char buffer[10];
    int *misaligned_ptr = (int*)(buffer + 1); // Desalinhado
    
    printf("Tentando acesso desalinhado...\n");
    *misaligned_ptr = 0x12345678; // Pode causar SIGBUS
    
    printf("Acesso desalinhado bem-sucedido (arquitetura tolerante)\n");
}

void test_double_free() {
    printf("=== CAUSANDO ERRO DE DOUBLE FREE ===\n");
    
    char *ptr = malloc(100);
    if (ptr) {
        strcpy(ptr, "Dados de teste");
        printf("Dados: %s\n", ptr);
        
        free(ptr);
        printf("Primeira liberação OK\n");
        
        // Segunda liberação causa erro
        free(ptr); // ERRO: Double free!
    }
}

void test_use_after_free() {
    printf("=== CAUSANDO USO APÓS LIBERAÇÃO ===\n");
    
    char *ptr = malloc(100);
    if (ptr) {
        strcpy(ptr, "Dados originais");
        printf("Dados antes: %s\n", ptr);
        
        free(ptr);
        printf("Memória liberada\n");
        
        // Uso após liberação - comportamento indefinido
        strcpy(ptr, "Dados após free"); // ERRO: Use after free!
        printf("Dados após free: %s\n", ptr);
    }
}

int main(int argc, char *argv[]) {
    printf("=== DEMONSTRAÇÃO: CORE DUMPS E OUTROS ERROS ===\n");
    printf("Este programa causará terminação anormal\n\n");
    
    enable_core_dumps();
    
    int option = 1;
    if (argc > 1) {
        option = atoi(argv[1]);
    }
    
    printf("Executando teste %d em 3 segundos...\n", option);
    sleep(3);
    
    switch(option) {
        case 1:
            cause_sigsegv();
            break;
        case 2:
            cause_sigfpe();
            break;
        case 3:
            cause_sigill();
            break;
        case 4:
            cause_sigabrt();
            break;
        case 5:
            cause_stackoverflow_signal();
            break;
        case 6:
            cause_bus_error();
            break;
        case 7:
            test_double_free();
            break;
        case 8:
            test_use_after_free();
            break;
        default:
            printf("Opções:\n");
            printf("1=SIGSEGV, 2=SIGFPE, 3=SIGILL, 4=SIGABRT\n");
            printf("5=Stack overflow, 6=Bus error, 7=Double free, 8=Use after free\n");
            cause_sigsegv();
    }
    
    printf("Se você está lendo isto, o erro foi evitado!\n");
    return 0;
}

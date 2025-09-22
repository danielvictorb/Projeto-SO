/*
 * Exemplo de Race Condition (Condições de Corrida)
 * 
 * Este programa demonstra condições de corrida entre threads
 * que acessam recursos compartilhados sem sincronização adequada.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Variável global compartilhada (causa race condition)
int shared_counter = 0;
int shared_array[1000];

// Estrutura para passar dados para as threads
typedef struct {
    int thread_id;
    int iterations;
} thread_data_t;

void* increment_counter(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    
    printf("Thread %d iniciada\n", data->thread_id);
    
    for (int i = 0; i < data->iterations; i++) {
        // RACE CONDITION: múltiplas threads modificando shared_counter simultaneamente
        // sem sincronização (mutex)
        int temp = shared_counter;
        usleep(1); // Simula algum processamento
        shared_counter = temp + 1;
        
        // Também modifica o array compartilhado
        if (i < 1000) {
            shared_array[i] = data->thread_id;
        }
    }
    
    printf("Thread %d finalizada\n", data->thread_id);
    return NULL;
}

void* bank_account_simulation(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    static int account_balance = 1000; // Saldo da conta bancária
    
    printf("Thread bancária %d iniciada\n", data->thread_id);
    
    for (int i = 0; i < data->iterations; i++) {
        // Simula operação bancária sem lock
        int current_balance = account_balance;
        usleep(rand() % 1000); // Simula processamento variável
        
        if (data->thread_id % 2 == 0) {
            // Threads pares fazem depósitos
            account_balance = current_balance + 10;
            printf("Thread %d: Depósito +10, saldo: %d\n", data->thread_id, account_balance);
        } else {
            // Threads ímpares fazem saques
            if (current_balance >= 10) {
                account_balance = current_balance - 10;
                printf("Thread %d: Saque -10, saldo: %d\n", data->thread_id, account_balance);
            }
        }
    }
    
    return NULL;
}

void test_counter_race() {
    const int num_threads = 5;
    const int iterations_per_thread = 1000;
    
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];
    
    printf("=== TESTE 1: RACE CONDITION NO CONTADOR ===\n");
    printf("Criando %d threads, cada uma incrementando %d vezes\n", num_threads, iterations_per_thread);
    printf("Valor esperado final: %d\n", num_threads * iterations_per_thread);
    
    // Cria as threads
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].iterations = iterations_per_thread;
        
        if (pthread_create(&threads[i], NULL, increment_counter, &thread_data[i]) != 0) {
            perror("Erro ao criar thread");
            exit(1);
        }
    }
    
    // Espera todas as threads terminarem
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Valor final do contador: %d\n", shared_counter);
    printf("Diferença devido à race condition: %d\n", (num_threads * iterations_per_thread) - shared_counter);
}

void test_bank_race() {
    const int num_threads = 4;
    const int transactions = 50;
    
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];
    
    printf("\n=== TESTE 2: RACE CONDITION BANCÁRIA ===\n");
    printf("Simulando operações bancárias concorrentes\n");
    
    // Cria as threads
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].iterations = transactions;
        
        if (pthread_create(&threads[i], NULL, bank_account_simulation, &thread_data[i]) != 0) {
            perror("Erro ao criar thread");
            exit(1);
        }
    }
    
    // Espera todas as threads terminarem
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main(int argc, char *argv[]) {
    printf("=== DEMONSTRAÇÃO: RACE CONDITIONS ===\n");
    printf("Este programa demonstra condições de corrida entre threads\n\n");
    
    int option = 1;
    if (argc > 1) {
        option = atoi(argv[1]);
    }
    
    switch(option) {
        case 1:
            test_counter_race();
            break;
        case 2:
            test_bank_race();
            break;
        case 3:
            test_counter_race();
            test_bank_race();
            break;
        default:
            printf("Opções: 1=contador, 2=banco, 3=ambos\n");
            test_counter_race();
    }
    
    return 0;
}

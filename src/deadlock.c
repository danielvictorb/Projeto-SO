/*
 * Exemplo de Deadlock
 * 
 * Este programa demonstra diferentes tipos de deadlocks entre threads,
 * incluindo deadlock clássico de dois mutex e deadlocks mais complexos.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <pthread.h>
 #include <unistd.h>
 
 // Dois mutex para demonstrar deadlock clássico
 pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
 pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;
 
 // Recursos compartilhados protegidos pelos mutex
 int resource_a = 0;
 int resource_b = 0;
 
 typedef struct {
     int thread_id;
 } thread_data_t;
 
 void* thread_function_1(void* arg) {
     thread_data_t* data = (thread_data_t*)arg;
     
     printf("Thread %d: Tentando adquirir mutex A...\n", data->thread_id);
     pthread_mutex_lock(&mutex_a);
     printf("Thread %d: Mutex A adquirido!\n", data->thread_id);
     
     resource_a++;
     printf("Thread %d: Modificando recurso A = %d\n", data->thread_id, resource_a);
     
     // Simula algum processamento
     sleep(2);
     
     printf("Thread %d: Tentando adquirir mutex B...\n", data->thread_id);
     pthread_mutex_lock(&mutex_b); // DEADLOCK! Thread 2 já tem mutex_b
     printf("Thread %d: Mutex B adquirido!\n", data->thread_id);
     
     resource_b++;
     printf("Thread %d: Modificando recurso B = %d\n", data->thread_id, resource_b);
     
     pthread_mutex_unlock(&mutex_b);
     pthread_mutex_unlock(&mutex_a);
     
     printf("Thread %d: Finalizando\n", data->thread_id);
     return NULL;
 }
 
 void* thread_function_2(void* arg) {
     thread_data_t* data = (thread_data_t*)arg;
     
     printf("Thread %d: Tentando adquirir mutex B...\n", data->thread_id);
     pthread_mutex_lock(&mutex_b);
     printf("Thread %d: Mutex B adquirido!\n", data->thread_id);
     
     resource_b += 10;
     printf("Thread %d: Modificando recurso B = %d\n", data->thread_id, resource_b);
     
     // Simula algum processamento
     sleep(2);
     
     printf("Thread %d: Tentando adquirir mutex A...\n", data->thread_id);
     pthread_mutex_lock(&mutex_a); // DEADLOCK! Thread 1 já tem mutex_a
     printf("Thread %d: Mutex A adquirido!\n", data->thread_id);
     
     resource_a += 10;
     printf("Thread %d: Modificando recurso A = %d\n", data->thread_id, resource_a);
     
     pthread_mutex_unlock(&mutex_a);
     pthread_mutex_unlock(&mutex_b);
     
     printf("Thread %d: Finalizando\n", data->thread_id);
     return NULL;
 }
 
 // Deadlock mais complexo com múltiplos recursos
 pthread_mutex_t mutex_pool[5] = {
     PTHREAD_MUTEX_INITIALIZER,
     PTHREAD_MUTEX_INITIALIZER,
     PTHREAD_MUTEX_INITIALIZER,
     PTHREAD_MUTEX_INITIALIZER,
     PTHREAD_MUTEX_INITIALIZER
 };
 
 void* complex_deadlock_thread(void* arg) {
     thread_data_t* data = (thread_data_t*)arg;
     int first_mutex = data->thread_id % 5;
     int second_mutex = (data->thread_id + 2) % 5;
     int third_mutex = (data->thread_id + 3) % 5;
     
     printf("Thread %d: Tentando adquirir mutex %d, %d, %d\n", 
            data->thread_id, first_mutex, second_mutex, third_mutex);
     
     // Adquire os mutex em ordem que pode causar deadlock
     pthread_mutex_lock(&mutex_pool[first_mutex]);
     printf("Thread %d: Adquiriu mutex %d\n", data->thread_id, first_mutex);
     sleep(1);
     
     pthread_mutex_lock(&mutex_pool[second_mutex]);
     printf("Thread %d: Adquiriu mutex %d\n", data->thread_id, second_mutex);
     sleep(1);
     
     pthread_mutex_lock(&mutex_pool[third_mutex]);
     printf("Thread %d: Adquiriu mutex %d\n", data->thread_id, third_mutex);
     
     // Simula trabalho crítico
     sleep(2);
     
     // Libera os mutex
     pthread_mutex_unlock(&mutex_pool[third_mutex]);
     pthread_mutex_unlock(&mutex_pool[second_mutex]);
     pthread_mutex_unlock(&mutex_pool[first_mutex]);
     
     printf("Thread %d: Liberou todos os mutex\n", data->thread_id);
     return NULL;
 }
 
 void test_simple_deadlock() {
     pthread_t thread1, thread2;
     thread_data_t data1 = {1};
     thread_data_t data2 = {2};
     
     printf("=== TESTE 1: DEADLOCK SIMPLES (2 MUTEX) ===\n");
     printf("Criando duas threads que adquirem mutex em ordem diferente...\n");
     
     // Cria as duas threads que causarão deadlock
     if (pthread_create(&thread1, NULL, thread_function_1, &data1) != 0) {
         perror("Erro ao criar thread 1");
         exit(1);
     }
     
     if (pthread_create(&thread2, NULL, thread_function_2, &data2) != 0) {
         perror("Erro ao criar thread 2");
         exit(1);
     }
     
     printf("Threads criadas. Deadlock deve ocorrer em alguns segundos...\n");
     printf("Use Ctrl+C para terminar quando o deadlock ocorrer.\n");
     
     // Espera as threads (que nunca terminarão devido ao deadlock)
     pthread_join(thread1, NULL);
     pthread_join(thread2, NULL);
 }
 
 void test_complex_deadlock() {
     const int num_threads = 5;
     pthread_t threads[num_threads];
     thread_data_t thread_data[num_threads];
     
     printf("=== TESTE 2: DEADLOCK COMPLEXO (MÚLTIPLOS MUTEX) ===\n");
     printf("Criando %d threads com padrão circular de uso de mutex...\n", num_threads);
     
     // Cria múltiplas threads
     for (int i = 0; i < num_threads; i++) {
         thread_data[i].thread_id = i;
         
         if (pthread_create(&threads[i], NULL, complex_deadlock_thread, &thread_data[i]) != 0) {
             perror("Erro ao criar thread");
             exit(1);
         }
     }
     
     printf("Threads criadas. Deadlock complexo pode ocorrer...\n");
     printf("Use Ctrl+C para terminar quando o deadlock ocorrer.\n");
     
     // Espera todas as threads
     for (int i = 0; i < num_threads; i++) {
         pthread_join(threads[i], NULL);
     }
 }
 
 int main(int argc, char *argv[]) {
     printf("=== DEMONSTRAÇÃO: DEADLOCKS ===\n");
     printf("Este programa demonstra diferentes tipos de deadlocks\n");
     printf("AVISO: O programa pode travar indefinidamente!\n\n");
     
     int option = 1;
     if (argc > 1) {
         option = atoi(argv[1]);
     }
     
     switch(option) {
         case 1:
             test_simple_deadlock();
             break;
         case 2:
             test_complex_deadlock();
             break;
         default:
             printf("Opções: 1=deadlock simples, 2=deadlock complexo\n");
             test_simple_deadlock();
     }
     
     printf("Se você está lendo isto, o deadlock foi evitado (inesperado)!\n");
     return 0;
 }
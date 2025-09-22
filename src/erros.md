# 🔥 Resumo dos Erros de Execução - Apresentação

Este documento resume todos os erros implementados no projeto, indicando as **linhas específicas** que causam cada problema e explicando **por que** acontecem.

---

## 📋 **1. STACK OVERFLOW**
**Arquivo:** `src/stack_overflow.c`

### **Linhas Problemáticas:**
- **Linha 18:** `recursive_function(depth + 1);`
- **Linha 13:** `char buffer[1024];` (a cada chamada recursiva)

### **Por que acontece:**
- **Recursão infinita** sem condição de parada
- A cada chamada, aloca **1024 bytes** no stack
- Stack tem tamanho limitado (~8MB no Linux)
- Após ~8000 chamadas, estoura o stack

### **Resultado:**
- **SIGSEGV** (Segmentation Fault)
- Processo termina com "Stack overflow"

---

## ⚡ **2. SEGMENTATION FAULT**  
**Arquivo:** `src/segmentation_fault.c`

### **2.1 Ponteiro Nulo (Função: `null_pointer_access`)**
- **Linha problema:** `17: *ptr = 42;`
- **Por que:** Tentativa de escrever em endereço 0x0 (NULL)
- **Resultado:** SIGSEGV imediato

### **2.2 Memória Inválida (Função: `invalid_memory_access`)**  
- **Linha problema:** `27: *ptr = 100;`
- **Por que:** Acessa endereço `0x12345678` que não pertence ao processo
- **Resultado:** SIGSEGV por violação de proteção de memória

### **2.3 Violação de Bounds (Função: `array_bounds_violation`)**
- **Linha problema:** `37: arr[10000] = 42;`
- **Por que:** Array tem 10 elementos, mas acessa posição 10000
- **Resultado:** Escreve em memória não pertencente ao array

---

## 💾 **3. BUFFER OVERFLOW**
**Arquivo:** `src/buffer_overflow.c`

### **3.1 Stack Buffer Overflow (Função: `stack_buffer_overflow`)**
- **Linha problema:** `18: strcpy(buffer, "Esta string é muito maior...");`
- **Por que:** Buffer de 10 bytes, string de 77 caracteres
- **Resultado:** Sobrescreve variáveis adjacentes e pode corromper endereço de retorno

### **3.2 Heap Buffer Overflow (Função: `heap_buffer_overflow`)**
- **Linha problema:** `33: strcpy(buffer, "String muito longa...");`
- **Por que:** Buffer heap de 10 bytes, string de 60+ caracteres  
- **Resultado:** Corrompe metadata do heap, pode causar crashes

### **3.3 Format String Vulnerability (Função: `format_string_vulnerability`)**
- **Linha problema:** `47: printf(user_input);`
- **Por que:** Usa string com `%s %s %s...` diretamente como format string
- **Resultado:** Lê memória da pilha como ponteiros, pode vazar dados

---

## 🕳️ **4. MEMORY LEAK**
**Arquivo:** `src/memory_leak.c`

### **4.1 Vazamento Simples (Função: `simple_memory_leak`)**
- **Linhas problema:** 
  - `18: char *leaked_memory = malloc(1024);` (aloca)
  - `21: // free(leaked_memory);` (comentado - não libera!)
- **Por que:** Loop aloca 1000x1024 bytes (1MB total) mas nunca libera
- **Resultado:** 1MB de vazamento permanente

### **4.2 Vazamento Recursivo (Função: `recursive_memory_leak`)**
- **Linhas problema:**
  - `35: char *buffer = malloc(2048);` (aloca)  
  - `43: // free(buffer);` (comentado - não libera!)
- **Por que:** 20 chamadas recursivas, cada uma aloca 2KB sem liberar
- **Resultado:** 40KB vazados + crescimento da pilha

### **4.3 Vazamento Crescente (Função: `growing_memory_leak`)**
- **Linhas problema:**
  - `52: void *ptr = malloc(size);` (aloca tamanhos crescentes)
  - **Sem `free()` em lugar nenhum**
- **Por que:** Aloca 1KB, 2KB, 3KB... até 50KB (total: 1.3MB)
- **Resultado:** Vazamento progressivo observável no `top`

---

## 🏃‍♂️ **5. RACE CONDITION**
**Arquivo:** `src/race_condition.c`

### **5.1 Contador Compartilhado (Função: `increment_counter`)**
- **Linhas problema:**
  - `31: int temp = shared_counter;`
  - `32: usleep(1);` (delay intencional)
  - `33: shared_counter = temp + 1;`
- **Por que:** 
  - Operação **read-modify-write não atômica**
  - 5 threads fazem isso simultaneamente
  - Sem mutex/semáforo para sincronizar
- **Resultado:** Valor final incorreto (< 5000 esperado)

### **5.2 Simulação Bancária (Função: `bank_account_simulation`)**
- **Linhas problema:**
  - `53: int current_balance = account_balance;`
  - `54: usleep(rand() % 1000);` (delay aleatório)  
  - `58/63: account_balance = current_balance ± 10;`
- **Por que:** Múltiplas threads leem saldo antigo e escrevem baseado nele
- **Resultado:** Transações perdidas, saldo inconsistente

---

## 🔒 **6. DEADLOCK**
**Arquivo:** `src/deadlock.c`

### **6.1 Deadlock Simples**
**Thread 1:**
- **Linhas:** `29: lock(mutex_a)` → `39: lock(mutex_b)`

**Thread 2:**  
- **Linhas:** `56: lock(mutex_b)` → `66: lock(mutex_a)`

### **Por que acontece:**
- **Thread 1** pega `mutex_a`, espera `mutex_b`
- **Thread 2** pega `mutex_b`, espera `mutex_a`  
- **Espera circular** - nenhuma pode prosseguir
- **Resultado:** Programa trava indefinidamente

### **6.2 Deadlock Complexo (Função: `complex_deadlock_thread`)**
- **Linhas problema:** `98, 102, 106` - locks em ordens diferentes
- **Por que:** 5 threads pegam 3 mutex cada, em ordens que criam ciclos
- **Resultado:** Deadlock mais difícil de detectar

---

## 💀 **7. CORE DUMP**  
**Arquivo:** `src/core_dump.c`

### **7.1 SIGSEGV (Função: `cause_sigsegv`)**
- **Linha:** `*null_ptr = 42;` 
- **Sinal:** SIGSEGV (11)

### **7.2 SIGFPE (Função: `cause_sigfpe`)**
- **Linha:** `int result = a / b;` (onde b = 0)
- **Sinal:** SIGFPE (8) - Divisão por zero

### **7.3 SIGILL (Função: `cause_sigill`)**  
- **Linha:** `illegal_func();` (executa bytes inválidos)
- **Sinal:** SIGILL (4) - Instrução ilegal

### **7.4 SIGABRT (Função: `cause_sigabrt`)**
- **Linha:** `abort();`
- **Sinal:** SIGABRT (6) - Terminação forçada

### **7.5 Stack Overflow Signal (Função: `recursive_bomb`)**
- **Linha:** `recursive_bomb();` (recursão infinita)
- **Buffer:** `char huge_buffer[10000];` a cada chamada
- **Resultado:** SIGSEGV quando stack estoura

### **7.6 Double Free (Função: `test_double_free`)**
- **Linhas problema:**
  - `free(ptr);` (primeira liberação OK)
  - `free(ptr);` (segunda liberação - ERRO!)
- **Por que:** Liberar memória já liberada corrompe heap
- **Resultado:** SIGABRT ou corrupção

### **7.7 Use After Free (Função: `test_use_after_free`)**
- **Linhas problema:**
  - `free(ptr);` (libera memória)
  - `strcpy(ptr, "...");` (usa memória liberada!)
- **Por que:** Memória pode ter sido realocada para outro uso
- **Resultado:** Comportamento indefinido, possível corrupção

---

## 🎯 **RESUMO PARA APRESENTAÇÃO**

### **Principais Causas de Erros:**
1. **Gestão de Memória** - Não liberar malloc(), usar após free()
2. **Ponteiros Inválidos** - NULL, endereços inválidos  
3. **Concorrência** - Falta de sincronização entre threads
4. **Limites de Buffer** - Escrever além dos limites
5. **Recursos Finitos** - Esgotar stack, criar dependências circulares

### **Como Detectar:**
- **Valgrind** - Memory leaks, invalid access
- **GDB** - Debugging de crashes
- **AddressSanitizer** - Buffer overflows
- **ThreadSanitizer** - Race conditions
- **Helgrind** - Deadlocks

### **Consequências:**
- **Crashes** - SIGSEGV, SIGABRT
- **Vazamentos** - Consumo crescente de RAM  
- **Inconsistência** - Dados corrompidos
- **Travamentos** - Deadlocks
- **Vulnerabilidades** - Exploração de buffers

---

**💡 Dica:** Use o comando `./scripts/run_error_simulator.sh` para demonstrar cada erro de forma controlada durante a apresentação!

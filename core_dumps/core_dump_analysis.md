# Análise de Core Dump - Exemplo Educativo

## O que é um Core Dump?

Um core dump é um arquivo que contém o estado completo da memória de um processo no momento em que ele falhou. Ele inclui:

1. **Memória do processo**: Stack, heap, segmento de dados
2. **Registros da CPU**: Estado dos registros no momento da falha
3. **Informações de contexto**: PID, signal que causou a falha, etc.

## Conteúdo Típico de um Core Dump

### 1. Cabeçalho ELF (Linux) ou Mach-O (macOS)
```
Magic: ELF 64-bit LSB core file
Type: CORE (Core file)
Machine: ARM64 ou x86-64
```

### 2. Símbolos e Endereços das Funções
Com base no nosso programa:
```
0000000100000000 T __mh_execute_header
00000001000004e8 T _cause_sigsegv      <-- Função que causa SIGSEGV
0000000100000538 T _cause_sigfpe       <-- Função que causa SIGFPE
00000001000005c4 T _cause_sigill       <-- Função que causa SIGILL
0000000100000618 T _cause_sigabrt      <-- Função que causa SIGABRT
00000001000006d4 T _cause_stackoverflow_signal
0000000100000704 T _cause_bus_error
00000001000008d4 T _main               <-- Função principal
```

### 3. Strings na Memória
Strings que estariam presentes no core dump:
```
"Core dumps habilitados"
"=== CAUSANDO SIGSEGV (Core Dump) ==="
"Tentando acessar ponteiro nulo..."
"Sinal recebido: %d"
"=== CAUSANDO SIGFPE (Floating Point Exception) ==="
"Tentando divisão por zero: %d / %d"
```

### 4. Stack Trace (Pilha de Chamadas)
```
#0  0x000000010000051c in cause_sigsegv () at src/core_dump.c:42
#1  0x00000001000009a8 in main (argc=2, argv=0x16fdff2a8) at src/core_dump.c:163
#2  0x000000018065e0e0 in start () from /usr/lib/dyld
```

### 5. Registros da CPU (exemplo ARM64)
```
x0  = 0x0000000000000000  (ponteiro nulo que causou o segfault)
x1  = 0x000000000000002a  (valor 42 que tentamos escrever)
sp  = 0x000000016fdff280  (stack pointer)
lr  = 0x00000001000009a8  (link register - endereço de retorno)
pc  = 0x000000010000051c  (program counter - onde a falha ocorreu)
```

### 6. Mapeamento de Memória
```
Start Address    End Address      Size     Permissions  Description
0x100000000     0x100001000      4096     r-x          Código executável
0x100001000     0x100002000      4096     rw-          Dados e variáveis globais
0x16fdff000     0x16fe00000      4096     rw-          Stack
0x600000000000  0x600000001000   4096     rw-          Heap (malloc)
```

## Ferramentas de Análise

### 1. file - Identificar o tipo
```bash
file core.12345
# Saída: core.12345: ELF 64-bit LSB core file ARM64
```

### 2. gdb - Análise detalhada
```bash
gdb programa core.12345
(gdb) bt           # Stack trace
(gdb) info registers  # Registros
(gdb) x/10i $pc    # Instruções ao redor da falha
```

### 3. hexdump - Conteúdo raw
```bash
hexdump -C core.12345 | head -20
```

### 4. strings - Extrair texto
```bash
strings core.12345 | grep -i "error\|segv\|abort"
```

## Exemplo de Saída do GDB

```
Program terminated with signal SIGSEGV, Segmentation fault.
#0  0x000000010000051c in cause_sigsegv () at core_dump.c:42
42      *null_ptr = 42; // SIGSEGV aqui!

(gdb) bt
#0  0x000000010000051c in cause_sigsegv () at core_dump.c:42
#1  0x00000001000009a8 in main (argc=2, argv=0x16fdff2a8) at core_dump.c:163

(gdb) info registers
x0             0x0      0
x1             0x2a     42
...
pc             0x10000051c  0x10000051c <cause_sigsegv+52>
```

## Informações Específicas por Tipo de Erro

### SIGSEGV (Segmentation Fault)
- **Causa**: Acesso inválido à memória
- **No core dump**: Endereço inválido nos registros
- **Exemplo**: `x0 = 0x0000000000000000` (NULL pointer)

### SIGFPE (Floating Point Exception) 
- **Causa**: Divisão por zero ou overflow
- **No core dump**: Operandos da divisão na stack
- **Exemplo**: `a = 10, b = 0` na memória

### SIGABRT (Abort)
- **Causa**: Chamada explícita de abort()
- **No core dump**: Call stack mostra abort()
- **Exemplo**: Função `abort()` na pilha de chamadas

Este é o tipo de informação que você encontraria em um core dump real!

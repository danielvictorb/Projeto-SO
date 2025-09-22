# 🔥 Emulador de Erros de Execução

Projeto para a disciplina de Sistemas Operacionais que demonstra diferentes tipos de erros de execução em sistemas operacionais Linux, implementado em C e executado em ambiente containerizado.

## 📋 Descrição

Este emulador demonstra de forma controlada os principais tipos de erros que podem ocorrer durante a execução de programas, incluindo:

- **Stack Overflow** - Estouro da pilha de execução
- **Segmentation Fault** - Violação de acesso à memória
- **Buffer Overflow** - Estouro de buffer com diferentes variações
- **Memory Leak** - Vazamento de memória
- **Race Condition** - Condições de corrida entre threads
- **Deadlock** - Bloqueio mútuo entre threads
- **Core Dump** - Diversos sinais que causam dump de memória

## 🏗️ Estrutura do Projeto

```
Projeto-SO/
├── src/                     # Código fonte dos exemplos
│   ├── stack_overflow.c     # Demonstração de stack overflow
│   ├── segmentation_fault.c # Exemplos de segfault
│   ├── buffer_overflow.c    # Buffer overflows variados
│   ├── memory_leak.c        # Vazamentos de memória
│   ├── race_condition.c     # Condições de corrida
│   ├── deadlock.c           # Deadlocks entre threads
│   └── core_dump.c          # Sinais e core dumps
├── scripts/                 # Scripts de automação
│   ├── run_error_simulator.sh  # Script principal (menu interativo)
│   └── docker_runner.sh     # Gerenciador Docker
├── bin/                     # Executáveis compilados
├── core_dumps/             # Diretório para core dumps
├── Dockerfile              # Container para execução isolada
├── docker-compose.yml      # Configuração Docker Compose
├── Makefile               # Automação da compilação
└── README.md             # Documentação (este arquivo)
```

## 🚀 Como Usar

### Opção 1: Execução Direta (Linux)

```bash
# 1. Clone e navegue para o diretório
cd Projeto-SO

# 2. Compile os exemplos
make all

# 3. Execute o menu interativo
./scripts/run_error_simulator.sh

# OU execute diretamente um tipo específico
./scripts/run_error_simulator.sh segfault 1
./scripts/run_error_simulator.sh deadlock 1
```

### Opção 2: Execução com Docker (Recomendado)

```bash
# 1. Execução interativa com Docker
./scripts/docker_runner.sh

# 2. Usando Docker Compose
./scripts/docker_runner.sh compose

# 3. Comando específico
./scripts/docker_runner.sh cmd "./bin/segmentation_fault 1"

# 4. Limpeza
./scripts/docker_runner.sh clean
```

## 🧪 Tipos de Erros Demonstrados

### 1. Stack Overflow
- **Arquivo**: `src/stack_overflow.c`
- **Causa**: Recursão infinita que esgota a pilha
- **Demonstração**: Função recursiva sem condição de parada

### 2. Segmentation Fault
- **Arquivo**: `src/segmentation_fault.c`
- **Variações**:
  1. Acesso a ponteiro nulo
  2. Acesso a memória inválida
  3. Violação de bounds de array

### 3. Buffer Overflow
- **Arquivo**: `src/buffer_overflow.c`
- **Variações**:
  1. Stack buffer overflow
  2. Heap buffer overflow
  3. Format string vulnerability

### 4. Memory Leak
- **Arquivo**: `src/memory_leak.c`
- **Variações**:
  1. Vazamento simples
  2. Vazamento recursivo
  3. Vazamento crescente

### 5. Race Condition
- **Arquivo**: `src/race_condition.c`
- **Variações**:
  1. Contador compartilhado sem sincronização
  2. Simulação de operações bancárias concorrentes

### 6. Deadlock
- **Arquivo**: `src/deadlock.c`
- **Variações**:
  1. Deadlock simples (2 mutex)
  2. Deadlock complexo (múltiplos mutex)

### 7. Core Dump
- **Arquivo**: `src/core_dump.c`
- **Sinais demonstrados**:
  - SIGSEGV, SIGFPE, SIGILL, SIGABRT
  - Stack overflow, Bus error
  - Double free, Use after free

## 🔧 Compilação e Dependências

### Dependências
- **GCC/G++** - Compilador C/C++
- **Make** - Automação de build
- **pthread** - Biblioteca de threads POSIX
- **Docker** (opcional) - Para execução isolada

### Comandos Make Disponíveis

```bash
make all                    # Compila todos os exemplos
make clean                  # Remove arquivos compilados
make help                   # Mostra ajuda

# Testes individuais
make test-stack-overflow    # Testa stack overflow
make test-segfault         # Testa segmentation fault
make test-buffer-overflow  # Testa buffer overflow
make test-memory-leak      # Testa memory leak
make test-race-condition   # Testa race condition
make test-deadlock         # Testa deadlock
make test-core-dump        # Testa core dump

make test-all              # Executa todos os testes (CUIDADO!)
```

## 🐳 Docker

### Construir e Executar

```bash
# Usando script facilitador
./scripts/docker_runner.sh

# Ou manualmente
docker build -t error-simulator .
docker run -it --cap-add=SYS_PTRACE --security-opt seccomp=unconfined error-simulator
```

### Docker Compose

```bash
docker-compose up --build
```

## ⚠️ Avisos Importantes

### ⚡ **CUIDADO**: Este projeto causa erros intencionalmente!

- **Stack Overflow**: Pode esgotar a pilha e travar o processo
- **Segmentation Fault**: Causa terminação imediata do programa
- **Buffer Overflow**: Pode causar comportamento imprevisível
- **Memory Leak**: Consome memória progressivamente
- **Deadlock**: Pode travar o processo indefinidamente
- **Core Dump**: Gera arquivos de dump de memória

### 🛡️ Recomendações de Segurança

1. **Use Docker**: Execução isolada protege o sistema host
2. **Monitore recursos**: Use `htop`, `top` ou `free` para monitorar memória
3. **Tenha Ctrl+C pronto**: Para interromper processos travados
4. **Não execute em produção**: Apenas para fins educacionais
5. **Backup de dados**: Em caso de experimentos extremos

## 📊 Monitoramento

### Ferramentas Úteis

```bash
# Monitoramento de memória
top
htop
free -h
watch -n 1 'ps aux --sort=-%mem | head'

# Debugging
gdb ./bin/segmentation_fault
valgrind ./bin/memory_leak
strace ./bin/stack_overflow

# Core dumps
ulimit -c unlimited
ls -la core*
gdb programa core.dump
```

## 🎓 Uso Educacional

### Conceitos Demonstrados

1. **Gerenciamento de Memória**
   - Stack vs Heap
   - Alocação e liberação de memória
   - Ponteiros e referências

2. **Concorrência**
   - Threads e sincronização
   - Mutex e locks
   - Problemas de concorrência

3. **Segurança**
   - Vulnerabilidades de buffer
   - Exploração de memória
   - Proteções do sistema

4. **Sistema Operacional**
   - Sinais de sistema
   - Gerenciamento de processos
   - Debugging e core dumps

## 🔍 Análise dos Resultados

### Observações Esperadas

- **Stack Overflow**: Terminação com "Segmentation fault (core dumped)"
- **Segfault**: Acesso inválido causa SIGSEGV
- **Buffer Overflow**: Pode corromper dados adjacentes
- **Memory Leak**: Crescimento progressivo do uso de memória
- **Race Condition**: Resultados inconsistentes entre execuções
- **Deadlock**: Processo trava sem progresso

## 🚫 Limitações

- Alguns erros podem não ocorrer em todas as arquiteturas
- Compiladores modernos têm proteções que podem prevenir certos erros
- Sistemas com ASLR podem alterar o comportamento
- Containers Docker têm limitações de recursos

## 📚 Referências

- [Linux System Programming](https://man7.org/linux/man-pages/)
- [POSIX Threads Programming](https://hpc-tutorials.llnl.gov/posix/)
- [Memory Management](https://www.kernel.org/doc/html/latest/admin-guide/mm/)
- [Docker Security](https://docs.docker.com/engine/security/)

## 🤝 Contribuição

Este é um projeto educacional. Sugestões para melhorias:

1. Novos tipos de erros para demonstrar
2. Melhorias na documentação
3. Exemplos mais didáticos
4. Ferramentas de análise adicionais

## 👥 Autores e Contribuições

Este projeto foi desenvolvido em equipe, com cada membro responsável por partes específicas:

### 💻 **Daniel Victor**
- Implementação do código fonte (`src/`)
- Desenvolvimento dos exemplos de erros de execução em C
- Documentação técnica dos conceitos demonstrados

### 🐳 **Ícaro Mori**
- Configuração do ambiente Docker (`Dockerfile`)
- Orquestração com Docker Compose (`docker-compose.yml`)
- Configurações de segurança e isolamento de containers

### 🔧 **Vitor Reis**  
- Scripts de automação bash (`scripts/`)
- Sistema de build automatizado (`Makefile`)
- Interface de linha de comando e menu interativo

## 📄 Licença

Projeto desenvolvido para fins educacionais na cadeira de Sistemas Operacionais.

---

**⚠️ DISCLAIMER**: Este software é destinado APENAS para fins educacionais. O uso inadequado pode causar instabilidade do sistema ou perda de dados. Execute sempre em ambiente controlado!

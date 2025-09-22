# Makefile para o Emulador de Erros de Execução
# 
# Este Makefile compila todos os exemplos de erros de execução
# com as flags apropriadas para demonstrar cada tipo de erro.

CC = gcc
CXX = g++
CFLAGS = -Wall -g -O0
CXXFLAGS = -Wall -g -O0 -std=c++11
THREAD_FLAGS = -pthread
SRCDIR = src
BINDIR = bin

# Detectar comando de timeout disponível
TIMEOUT_CMD := $(shell \
	if command -v timeout >/dev/null 2>&1; then \
		echo "timeout"; \
	elif command -v gtimeout >/dev/null 2>&1; then \
		echo "gtimeout"; \
	else \
		echo "echo 'Aviso: timeout não disponível, executando sem limite de tempo' &&"; \
	fi \
)

# Lista de todos os executáveis
TARGETS = stack_overflow segmentation_fault buffer_overflow memory_leak \
          race_condition deadlock core_dump

# Diretório de saída para os executáveis
$(BINDIR):
	mkdir -p $(BINDIR)

# Regra padrão - compila todos os exemplos
all: $(BINDIR) $(addprefix $(BINDIR)/, $(TARGETS))
	@echo "=== COMPILAÇÃO CONCLUÍDA ==="
	@echo "Executáveis disponíveis em $(BINDIR)/:"
	@ls -la $(BINDIR)/

# Compilação dos exemplos simples (sem threads)
$(BINDIR)/stack_overflow: $(SRCDIR)/stack_overflow.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<
	@echo "✓ Stack overflow compilado"

$(BINDIR)/segmentation_fault: $(SRCDIR)/segmentation_fault.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<
	@echo "✓ Segmentation fault compilado"

$(BINDIR)/buffer_overflow: $(SRCDIR)/buffer_overflow.c | $(BINDIR)
	# Compilação sem proteções para demonstrar buffer overflow
	$(CC) $(CFLAGS) -fno-stack-protector -o $@ $<
	@echo "✓ Buffer overflow compilado (sem proteções)"

$(BINDIR)/memory_leak: $(SRCDIR)/memory_leak.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<
	@echo "✓ Memory leak compilado"

$(BINDIR)/core_dump: $(SRCDIR)/core_dump.c | $(BINDIR)
	$(CC) $(CFLAGS) -lm -o $@ $<
	@echo "✓ Core dump compilado"

# Compilação dos exemplos com threads
$(BINDIR)/race_condition: $(SRCDIR)/race_condition.c | $(BINDIR)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -o $@ $<
	@echo "✓ Race condition compilado"

$(BINDIR)/deadlock: $(SRCDIR)/deadlock.c | $(BINDIR)
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -o $@ $<
	@echo "✓ Deadlock compilado"

# Regras de limpeza
clean:
	rm -rf $(BINDIR)
	@echo "Arquivos compilados removidos"

# Regras para testar cada exemplo
test-stack-overflow: $(BINDIR)/stack_overflow
	@echo "=== TESTANDO STACK OVERFLOW ==="
	@echo "AVISO: Este teste causará stack overflow!"
	@echo "Executando em 3 segundos... (Ctrl+C para cancelar)"
	@sleep 3
	-$(TIMEOUT_CMD) 10s ./$(BINDIR)/stack_overflow || echo "Teste de stack overflow executado"

test-segfault: $(BINDIR)/segmentation_fault
	@echo "=== TESTANDO SEGMENTATION FAULT ==="
	@echo "AVISO: Este teste causará segmentation fault!"
	@echo "Executando em 3 segundos... (Ctrl+C para cancelar)"
	@sleep 3
	-./$(BINDIR)/segmentation_fault 1 || echo "Teste de segfault executado"

test-buffer-overflow: $(BINDIR)/buffer_overflow
	@echo "=== TESTANDO BUFFER OVERFLOW ==="
	@echo "AVISO: Este teste pode causar comportamento instável!"
	@echo "Executando em 3 segundos... (Ctrl+C para cancelar)"
	@sleep 3
	-./$(BINDIR)/buffer_overflow 1 || echo "Teste de buffer overflow executado"

test-memory-leak: $(BINDIR)/memory_leak
	@echo "=== TESTANDO MEMORY LEAK ==="
	@echo "Use 'top' ou 'htop' em outro terminal para monitorar o uso de memória"
	@echo "Executando teste de vazamento por 10 segundos..."
	-$(TIMEOUT_CMD) 10s ./$(BINDIR)/memory_leak 1 || echo "Teste de memory leak executado"

test-race-condition: $(BINDIR)/race_condition
	@echo "=== TESTANDO RACE CONDITION ==="
	./$(BINDIR)/race_condition 1

test-deadlock: $(BINDIR)/deadlock
	@echo "=== TESTANDO DEADLOCK ==="
	@echo "AVISO: Este teste pode travar indefinidamente!"
	@echo "Use Ctrl+C para terminar quando o deadlock ocorrer"
	@echo "Executando em 3 segundos... (Ctrl+C para cancelar)"
	@sleep 3
	-$(TIMEOUT_CMD) 30s ./$(BINDIR)/deadlock 1 || echo "Teste de deadlock executado (ou tempo limite atingido)"

test-core-dump: $(BINDIR)/core_dump
	@echo "=== TESTANDO CORE DUMP ==="
	@echo "AVISO: Este teste causará terminação anormal!"
	@echo "Executando em 3 segundos... (Ctrl+C para cancelar)"
	@sleep 3
	-./$(BINDIR)/core_dump 1 || echo "Teste de core dump executado"

# Regra para executar todos os testes (com cuidado!)
test-all: all
	@echo "=== EXECUTANDO TODOS OS TESTES ==="
	@echo "AVISO: Estes testes causarão erros intencionalmente!"
	@echo "Alguns podem travar o processo - use Ctrl+C se necessário"
	@echo ""
	$(MAKE) test-stack-overflow
	@echo ""
	$(MAKE) test-segfault
	@echo ""
	$(MAKE) test-buffer-overflow
	@echo ""
	$(MAKE) test-memory-leak
	@echo ""
	$(MAKE) test-race-condition
	@echo ""
	$(MAKE) test-deadlock
	@echo ""
	$(MAKE) test-core-dump
	@echo ""
	@echo "=== TODOS OS TESTES CONCLUÍDOS ==="

# Regra para mostrar ajuda
help:
	@echo "Emulador de Erros de Execução - Comandos Makefile:"
	@echo ""
	@echo "  make all              - Compila todos os exemplos"
	@echo "  make clean            - Remove arquivos compilados"
	@echo ""
	@echo "Testes individuais:"
	@echo "  make test-stack-overflow  - Testa stack overflow"
	@echo "  make test-segfault        - Testa segmentation fault"
	@echo "  make test-buffer-overflow - Testa buffer overflow"
	@echo "  make test-memory-leak     - Testa memory leak"
	@echo "  make test-race-condition  - Testa race condition"
	@echo "  make test-deadlock        - Testa deadlock"
	@echo "  make test-core-dump       - Testa core dump"
	@echo ""
	@echo "  make test-all         - Executa todos os testes (CUIDADO!)"
	@echo "  make help             - Mostra esta ajuda"

# Marca as regras que não criam arquivos
.PHONY: all clean test-stack-overflow test-segfault test-buffer-overflow \
        test-memory-leak test-race-condition test-deadlock test-core-dump \
        test-all help

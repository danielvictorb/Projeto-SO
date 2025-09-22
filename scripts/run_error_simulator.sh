#!/bin/bash

# Script para executar o Emulador de Erros de Execução
# 
# Este script fornece uma interface amigável para executar
# os diferentes tipos de erros de execução de forma controlada.

# Cores para output mais bonito
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Função para mostrar banner
show_banner() {
    echo -e "${BLUE}"
    echo "╔══════════════════════════════════════════════════════════════╗"
    echo "║                EMULADOR DE ERROS DE EXECUÇÃO                 ║"
    echo "║              Demonstração de Falhas de Sistema               ║"
    echo "╚══════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
}

# Função para mostrar menu principal
show_menu() {
    echo -e "${CYAN}Selecione o tipo de erro para simular:${NC}"
    echo ""
    echo -e "${GREEN}1)${NC} Stack Overflow"
    echo -e "${GREEN}2)${NC} Segmentation Fault"
    echo -e "${GREEN}3)${NC} Buffer Overflow"
    echo -e "${GREEN}4)${NC} Memory Leak"
    echo -e "${GREEN}5)${NC} Race Condition"
    echo -e "${GREEN}6)${NC} Deadlock"
    echo -e "${GREEN}7)${NC} Core Dump"
    echo -e "${GREEN}8)${NC} Executar Todos os Testes"
    echo -e "${GREEN}9)${NC} Compilar Exemplos"
    echo -e "${GREEN}0)${NC} Sair"
    echo ""
    echo -e "${YELLOW}Digite sua escolha [0-9]:${NC} "
}

# Função para executar comando com aviso
run_with_warning() {
    local command="$1"
    local warning="$2"
    
    echo -e "${RED}AVISO:${NC} $warning"
    echo -e "${YELLOW}Pressione Enter para continuar ou Ctrl+C para cancelar...${NC}"
    read -r
    
    echo -e "${PURPLE}Executando: $command${NC}"
    eval "$command"
    
    echo -e "${BLUE}Pressione Enter para continuar...${NC}"
    read -r
}

# Função para compilar se necessário
ensure_compiled() {
    if [ ! -d "bin" ] || [ -z "$(ls -A bin 2>/dev/null)" ]; then
        echo -e "${YELLOW}Compilando exemplos...${NC}"
        make all
        if [ $? -ne 0 ]; then
            echo -e "${RED}Erro na compilação! Verifique se você tem gcc instalado.${NC}"
            exit 1
        fi
    fi
}

# Função principal do menu interativo
interactive_menu() {
    while true; do
        clear
        show_banner
        show_menu
        
        read -r choice
        
        case $choice in
            1)
                ensure_compiled
                run_with_warning "timeout 10s ./bin/stack_overflow" \
                    "Este comando causará stack overflow e pode travar o processo!"
                ;;
            2)
                ensure_compiled
                echo -e "${CYAN}Escolha o tipo de segmentation fault:${NC}"
                echo "1) Ponteiro nulo"
                echo "2) Memória inválida"  
                echo "3) Array bounds"
                echo -e "${YELLOW}Digite [1-3]:${NC} "
                read -r subfault
                run_with_warning "./bin/segmentation_fault $subfault" \
                    "Este comando causará segmentation fault!"
                ;;
            3)
                ensure_compiled
                echo -e "${CYAN}Escolha o tipo de buffer overflow:${NC}"
                echo "1) Stack buffer overflow"
                echo "2) Heap buffer overflow"
                echo "3) Format string vulnerability"
                echo -e "${YELLOW}Digite [1-3]:${NC} "
                read -r suboverflow
                run_with_warning "./bin/buffer_overflow $suboverflow" \
                    "Este comando pode causar comportamento instável!"
                ;;
            4)
                ensure_compiled
                echo -e "${CYAN}Escolha o tipo de memory leak:${NC}"
                echo "1) Vazamento simples"
                echo "2) Vazamento recursivo"
                echo "3) Vazamento crescente"
                echo -e "${YELLOW}Digite [1-3]:${NC} "
                read -r subleak
                echo -e "${YELLOW}Use 'top' ou 'htop' em outro terminal para monitorar memória${NC}"
                run_with_warning "timeout 15s ./bin/memory_leak $subleak" \
                    "Este comando causará vazamento de memória!"
                ;;
            5)
                ensure_compiled
                echo -e "${CYAN}Escolha o tipo de race condition:${NC}"
                echo "1) Contador compartilhado"
                echo "2) Simulação bancária"
                echo "3) Ambos"
                echo -e "${YELLOW}Digite [1-3]:${NC} "
                read -r subrace
                run_with_warning "./bin/race_condition $subrace" \
                    "Este comando demonstrará condições de corrida entre threads!"
                ;;
            6)
                ensure_compiled
                echo -e "${CYAN}Escolha o tipo de deadlock:${NC}"
                echo "1) Deadlock simples (2 mutex)"
                echo "2) Deadlock complexo (múltiplos mutex)"
                echo -e "${YELLOW}Digite [1-2]:${NC} "
                read -r subdeadlock
                run_with_warning "timeout 30s ./bin/deadlock $subdeadlock" \
                    "Este comando pode travar indefinidamente devido ao deadlock!"
                ;;
            7)
                ensure_compiled
                echo -e "${CYAN}Escolha o tipo de core dump:${NC}"
                echo "1) SIGSEGV    5) Stack overflow"
                echo "2) SIGFPE     6) Bus error"
                echo "3) SIGILL     7) Double free"
                echo "4) SIGABRT    8) Use after free"
                echo -e "${YELLOW}Digite [1-8]:${NC} "
                read -r subcore
                run_with_warning "./bin/core_dump $subcore" \
                    "Este comando causará terminação anormal do processo!"
                ;;
            8)
                ensure_compiled
                run_with_warning "make test-all" \
                    "Isso executará TODOS os testes! Alguns podem travar o sistema!"
                ;;
            9)
                echo -e "${YELLOW}Compilando todos os exemplos...${NC}"
                make clean && make all
                echo -e "${GREEN}Compilação concluída!${NC}"
                echo -e "${BLUE}Pressione Enter para continuar...${NC}"
                read -r
                ;;
            0)
                echo -e "${GREEN}Saindo do emulador...${NC}"
                exit 0
                ;;
            *)
                echo -e "${RED}Opção inválida! Tente novamente.${NC}"
                sleep 2
                ;;
        esac
    done
}

# Função para mostrar ajuda
show_help() {
    echo "Emulador de Erros de Execução - Modo de Uso:"
    echo ""
    echo "  $0                    - Modo interativo (menu)"
    echo "  $0 [tipo] [opção]     - Execução direta"
    echo ""
    echo "Tipos disponíveis:"
    echo "  stack_overflow        - Demonstra stack overflow"
    echo "  segfault [1-3]       - Demonstra segmentation fault"
    echo "  buffer_overflow [1-3] - Demonstra buffer overflow"
    echo "  memory_leak [1-3]    - Demonstra memory leak"
    echo "  race_condition [1-3] - Demonstra race condition"
    echo "  deadlock [1-2]       - Demonstra deadlock"
    echo "  core_dump [1-8]      - Demonstra core dump"
    echo ""
    echo "Exemplos:"
    echo "  $0 segfault 1        - Executa segfault por ponteiro nulo"
    echo "  $0 deadlock 1        - Executa deadlock simples"
    echo "  $0 compile           - Apenas compila os exemplos"
    echo ""
}

# Verificar argumentos da linha de comando
if [ $# -eq 0 ]; then
    # Modo interativo
    interactive_menu
elif [ "$1" = "--help" ] || [ "$1" = "-h" ]; then
    show_help
elif [ "$1" = "compile" ]; then
    echo -e "${YELLOW}Compilando exemplos...${NC}"
    make clean && make all
else
    # Modo linha de comando
    ensure_compiled
    
    case "$1" in
        stack_overflow)
            echo -e "${RED}Executando stack overflow...${NC}"
            timeout 10s ./bin/stack_overflow
            ;;
        segfault)
            option=${2:-1}
            echo -e "${RED}Executando segmentation fault (tipo $option)...${NC}"
            ./bin/segmentation_fault "$option"
            ;;
        buffer_overflow)
            option=${2:-1}
            echo -e "${RED}Executando buffer overflow (tipo $option)...${NC}"
            ./bin/buffer_overflow "$option"
            ;;
        memory_leak)
            option=${2:-1}
            echo -e "${RED}Executando memory leak (tipo $option)...${NC}"
            timeout 10s ./bin/memory_leak "$option"
            ;;
        race_condition)
            option=${2:-1}
            echo -e "${RED}Executando race condition (tipo $option)...${NC}"
            ./bin/race_condition "$option"
            ;;
        deadlock)
            option=${2:-1}
            echo -e "${RED}Executando deadlock (tipo $option)...${NC}"
            timeout 30s ./bin/deadlock "$option"
            ;;
        core_dump)
            option=${2:-1}
            echo -e "${RED}Executando core dump (tipo $option)...${NC}"
            ./bin/core_dump "$option"
            ;;
        *)
            echo -e "${RED}Tipo inválido: $1${NC}"
            echo ""
            show_help
            exit 1
            ;;
    esac
fi

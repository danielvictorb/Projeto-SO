#!/bin/bash

# Script para executar o Emulador de Erros usando Docker
# 
# Este script facilita a execução do container Docker com
# todas as configurações necessárias para os testes.

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Nome da imagem e container
IMAGE_NAME="error-simulator"
CONTAINER_NAME="error-simulator-container"

# Função para mostrar banner
show_banner() {
    echo -e "${BLUE}"
    echo "╔════════════════════════════════════════════════════════════════╗"
    echo "║           EMULADOR DE ERROS - EXECUÇÃO EM CONTAINER           ║"
    echo "║              Ambiente Isolado para Demonstrações              ║"
    echo "╚════════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
}

# Função para verificar se Docker está disponível
check_docker() {
    if ! command -v docker &> /dev/null; then
        echo -e "${RED}Erro: Docker não encontrado!${NC}"
        echo "Por favor, instale o Docker antes de continuar."
        exit 1
    fi
    
    if ! docker info &> /dev/null; then
        echo -e "${RED}Erro: Docker daemon não está rodando!${NC}"
        echo "Por favor, inicie o Docker daemon."
        exit 1
    fi
}

# Função para construir a imagem
build_image() {
    echo -e "${YELLOW}Construindo imagem Docker...${NC}"
    
    if docker build -t "$IMAGE_NAME" .; then
        echo -e "${GREEN}✓ Imagem construída com sucesso!${NC}"
    else
        echo -e "${RED}✗ Erro na construção da imagem!${NC}"
        exit 1
    fi
}

# Função para criar diretório de core dumps
setup_core_dumps() {
    mkdir -p core_dumps
    echo -e "${GREEN}✓ Diretório core_dumps criado${NC}"
}

# Função para executar container interativo
run_interactive() {
    echo -e "${YELLOW}Iniciando container interativo...${NC}"
    
    # Remove container anterior se existir
    docker rm -f "$CONTAINER_NAME" &> /dev/null
    
    # Executa o container
    docker run -it --rm \
        --name "$CONTAINER_NAME" \
        --cap-add=SYS_PTRACE \
        --security-opt seccomp=unconfined \
        --memory="1g" \
        --memory-swap="1g" \
        -v "$(pwd)/core_dumps:/app/core_dumps" \
        -e MALLOC_CHECK_=2 \
        -e MALLOC_PERTURB_=165 \
        "$IMAGE_NAME"
}

# Função para executar comando específico
run_command() {
    local command="$1"
    
    echo -e "${YELLOW}Executando comando: $command${NC}"
    
    docker run --rm \
        --name "${CONTAINER_NAME}-cmd" \
        --cap-add=SYS_PTRACE \
        --security-opt seccomp=unconfined \
        --memory="1g" \
        --memory-swap="1g" \
        -v "$(pwd)/core_dumps:/app/core_dumps" \
        -e MALLOC_CHECK_=2 \
        -e MALLOC_PERTURB_=165 \
        "$IMAGE_NAME" \
        sh -c "$command"
}

# Função para executar com Docker Compose
run_compose() {
    echo -e "${YELLOW}Usando Docker Compose...${NC}"
    
    if command -v docker-compose &> /dev/null; then
        docker-compose up --build
    elif docker compose version &> /dev/null; then
        docker compose up --build
    else
        echo -e "${RED}Docker Compose não encontrado! Usando docker run...${NC}"
        run_interactive
    fi
}

# Função para limpar containers e imagens
cleanup() {
    echo -e "${YELLOW}Limpando containers e imagens...${NC}"
    
    # Para containers em execução
    docker stop "$CONTAINER_NAME" &> /dev/null
    docker stop "${CONTAINER_NAME}-cmd" &> /dev/null
    
    # Remove containers
    docker rm "$CONTAINER_NAME" &> /dev/null
    docker rm "${CONTAINER_NAME}-cmd" &> /dev/null
    
    # Remove imagem
    docker rmi "$IMAGE_NAME" &> /dev/null
    
    # Remove core dumps
    rm -rf core_dumps
    
    echo -e "${GREEN}✓ Limpeza concluída!${NC}"
}

# Função para mostrar logs do container
show_logs() {
    docker logs "$CONTAINER_NAME" 2>/dev/null || echo -e "${YELLOW}Container não está em execução${NC}"
}

# Função para mostrar ajuda
show_help() {
    echo "Emulador de Erros - Docker Runner"
    echo ""
    echo "Uso: $0 [opção]"
    echo ""
    echo "Opções:"
    echo "  run, start, interactive  - Executa container interativo (padrão)"
    echo "  compose                  - Executa com Docker Compose"
    echo "  build                    - Constrói apenas a imagem"
    echo "  cmd 'comando'           - Executa comando específico"
    echo "  logs                    - Mostra logs do container"
    echo "  cleanup, clean          - Limpa containers e imagens"
    echo "  help, -h, --help        - Mostra esta ajuda"
    echo ""
    echo "Exemplos:"
    echo "  $0                      - Execução interativa (menu)"
    echo "  $0 compose             - Usar Docker Compose"  
    echo "  $0 cmd './bin/segfault 1' - Executa segfault diretamente"
    echo "  $0 build               - Apenas constrói a imagem"
    echo "  $0 clean               - Limpa ambiente Docker"
    echo ""
}

# Função principal
main() {
    show_banner
    check_docker
    setup_core_dumps
    
    case "${1:-interactive}" in
        run|start|interactive)
            build_image
            run_interactive
            ;;
        compose)
            run_compose
            ;;
        build)
            build_image
            ;;
        cmd)
            if [ -z "$2" ]; then
                echo -e "${RED}Erro: Especifique um comando para executar${NC}"
                echo "Exemplo: $0 cmd './scripts/run_error_simulator.sh segfault 1'"
                exit 1
            fi
            build_image
            run_command "$2"
            ;;
        logs)
            show_logs
            ;;
        cleanup|clean)
            cleanup
            ;;
        help|-h|--help)
            show_help
            ;;
        *)
            echo -e "${RED}Opção inválida: $1${NC}"
            echo ""
            show_help
            exit 1
            ;;
    esac
}

# Executa função principal
main "$@"

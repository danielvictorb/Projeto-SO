# Dockerfile para o Emulador de Erros de Execução
# 
# Este container fornece um ambiente isolado para executar
# os exemplos de erros de execução sem afetar o sistema host.

FROM ubuntu:22.04

# Evita prompts interativos durante a instalação
ENV DEBIAN_FRONTEND=noninteractive

# Instala dependências necessárias
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    make \
    gdb \
    valgrind \
    strace \
    htop \
    procps \
    vim \
    nano \
    less \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Configura limites para permitir core dumps
RUN echo "* soft core unlimited" >> /etc/security/limits.conf && \
    echo "* hard core unlimited" >> /etc/security/limits.conf

# Cria usuário não-root para maior segurança
RUN useradd -m -s /bin/bash erroruser && \
    usermod -aG sudo erroruser

# Cria diretório de trabalho
WORKDIR /app

# Copia código fonte para o container
COPY src/ ./src/
COPY scripts/ ./scripts/
COPY Makefile ./

# Garante permissões corretas
RUN chown -R erroruser:erroruser /app && \
    chmod +x ./scripts/*.sh

# Muda para o usuário não-root
USER erroruser

# Compila todos os exemplos
RUN make all

# Configurações adicionais para debugging
ENV MALLOC_CHECK_=2
ENV MALLOC_PERTURB_=165

# Comando padrão - executa o menu interativo
CMD ["./scripts/run_error_simulator.sh"]

# Labels para documentação
LABEL maintainer="Sistema Operacional - Projeto" \
      description="Emulador de erros de execução para demonstração educacional" \
      version="1.0"

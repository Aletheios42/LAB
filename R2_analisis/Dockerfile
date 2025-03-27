FROM ubuntu:22.04

# Instalar dependencias
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    git \
    build-essential \
    pkg-config \
    libzip-dev \
    libssl-dev \
    libcapstone-dev \
    libmagic-dev \
    && rm -rf /var/lib/apt/lists/*

# Instalar Radare2 desde fuente (forma más confiable)
RUN git clone https://github.com/radareorg/radare2.git && \
    cd radare2 && \
    ./sys/install.sh && \
    cd .. && \
    rm -rf radare2

# Instalar r2pipe para Python
RUN pip3 install r2pipe

# Directorio de trabajo
WORKDIR /app

# Copiar el script de análisis
COPY r2_analisis_ls.py /app/

# Comando por defecto
CMD ["python3", "/app/r2_analisis_ls.py"]

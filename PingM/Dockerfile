# ---------- Builder Stage ----------
    FROM ubuntu:24.04 AS builder

    # Install build tools
    RUN apt-get update && \
        apt-get install -y \
        build-essential \
        cmake && \
        rm -rf /var/lib/apt/lists/*
    
    # Set working directory
    WORKDIR /app
    
    # Copy project files
    COPY . .
    
    # Configure and build
    RUN cmake -B build -S .
    RUN cmake --build build
    
    # ---------- Runtime Stage ----------
    FROM ubuntu:24.04

    LABEL org.opencontainers.image.title="PingM" \
          org.opencontainers.image.description="CLI wrapper around the system ping command that produces a network analysis report." \
          org.opencontainers.image.authors="Naga Teja Manikanta Boddu <nagatejamanikantab@gmail.com>" \
          org.opencontainers.image.source="https://github.com/nagatejamanikantaboddu/PingM" \
          org.opencontainers.image.licenses="MIT" \
          org.opencontainers.image.version="1.0.0"

    RUN apt-get update && apt-get install -y iputils-ping && rm -rf /var/lib/apt/lists/*

    # Working directory
    WORKDIR /app
    
    # Copy only the executable
    COPY --from=builder /app/build/pingM /app/pingM
    
    # Default command
    ENTRYPOINT ["./pingM"]
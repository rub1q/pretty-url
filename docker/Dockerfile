# build
FROM debian:bookworm-slim AS build

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends \ 
    build-essential \
    python3-full \
    pipx \
    cmake \
    libssl-dev \
    gcc-12 \
    g++-12 \
    g++-12-multilib \
    && \
    pipx install conan \
    && \
    ln -s ~/.local/bin/conan /usr/local/bin/conan \
    && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN mkdir -p ./build

WORKDIR /app/build

# init conan
RUN conan profile detect
RUN sed -i 's/compiler=.*/compiler=gcc/' ~/.conan2/profiles/default && \
    sed -i 's/compiler.version=.*/compiler.version=12/' ~/.conan2/profiles/default && \
    echo "compiler.cppstd=20" >> ~/.conan2/profiles/default
RUN conan install ../conanfile-docker.txt --build=missing -s build_type=Release --profile=default

# init cmake
RUN cmake \
    -DCMAKE_GENERATOR:STRING="Unix Makefiles" \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/x86_64-linux-gnu-g++-12 \
    -DCMAKE_BUILD_TYPE:STRING=Release ..

RUN cmake --build .

####################################################

# run
FROM debian:bookworm-slim AS run

RUN groupadd --system appgroup && \
    useradd --system --home-dir /home/appuser --shell /bin/bash -g appgroup appuser

WORKDIR /app

COPY --from=build /app/bin/Release/prettyurl .

RUN chown -R appuser:appgroup .
RUN mkdir -p /var/log/pretty-url && \
    chown -R appuser:appgroup /var/log/pretty-url

USER appuser

EXPOSE 8080

ENTRYPOINT ["/app/prettyurl"] 
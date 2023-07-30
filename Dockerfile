FROM gcc:latest

ENV DEBIAN_FRONTEND=noninteractive

# Install DPP
RUN apt update && apt install --no-install-recommends -y libssl-dev zlib1g-dev libsodium-dev libopus-dev cmake pkg-config git gh && apt-get clean && rm -rf /var/lib/apt/lists/*
RUN git clone https://github.com/brainboxdotcc/DPP /usr/src/DPP
WORKDIR /usr/src/DPP
RUN git checkout v10.0.24
WORKDIR /usr/src/DPP/build
RUN cmake .. -DDPP_BUILD_TEST=OFF
RUN make -j "$(nproc)"
RUN make install

# Build the dominic_discord_project
WORKDIR /workspace
COPY . .
WORKDIR ./build
RUN cmake ..
RUN make -j "$(nproc)"

# Reset workdir
WORKDIR /workspace

ENTRYPOINT [ "/bin/bash" ]
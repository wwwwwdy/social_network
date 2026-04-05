FROM --platform=linux/amd64 debian:13-slim AS builder

RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    cmake \
    git \
    pkg-config \
    ca-certificates \
    libssl-dev \
    libpq-dev \
    postgresql-client \
    zlib1g-dev \
    libpng-dev \
    libfreetype-dev \
    libfontconfig-dev \
    libx11-dev \
    libxcb1-dev \
    libexpat1-dev \
    && rm -rf /var/lib/apt/lists/*

RUN git clone --depth 1 https://github.com/ultimatepp/ultimatepp.git /opt/upp \
    && make -C /opt/upp -f umkMakefile

RUN mkdir -p /opt/upp/upphub \
    && git clone --depth 1 https://github.com/mirek-fidler/Skylark.git /opt/upp/upphub/Skylark

RUN mkdir -p /app/jwt \
	&& git clone https://github.com/Thalhammer/jwt-cpp.git /app/jwt/jwt-cpp \
	&& cd /app/jwt/jwt-cpp \
	&& git checkout df914eba76e3dda3ec6b05caef81b1da9ceea6f2
COPY ./extras/jwt.upp /app/jwt


RUN mkdir -p /app/bcrypt \
	&& git clone https://github.com/hilch/Bcrypt.cpp.git /app/bcrypt/Bcrypt.cpp \
	&& cd /app/bcrypt/Bcrypt.cpp \
	&& git checkout 0d18b6a
COPY ./extras/bcrypt.upp /app/bcrypt


WORKDIR /app
COPY ./social_network /app/social_network

RUN /opt/upp/umk /app,/opt/upp/uppsrc,/opt/upp/upphub/Skylark social_network CLANG -bs /app/social_network

FROM --platform=linux/amd64 debian:13-slim AS runtime


RUN apt-get update && apt-get install -y \
    ca-certificates \
	libssl3 \
	libpq5 \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /app/social_network/social_network /app/social_network

CMD ["/app/social_network"]

from ubuntu:latest

WORKDIR /src/

RUN apt-get update -y
RUN apt-get install -y curl \
     gcc \
     libpthread-stubs0-dev \
     net-tools

COPY ./src/ /src/

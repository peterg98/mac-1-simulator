FROM ubuntu:20.04

COPY . .

RUN sudo apt-get install -y g++ flex make

FROM ubuntu:20.04

COPY . .

RUN apt-get update
RUN apt-get install -y g++ flex make git

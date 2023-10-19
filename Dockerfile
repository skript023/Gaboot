FROM ubuntu:latest

WORKDIR /app

EXPOSE 8080

COPY out/build/WSL-GCC-Release/Gaboot ./

COPY config.json ./

CMD ["./Gaboot"]
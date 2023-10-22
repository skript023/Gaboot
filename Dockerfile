FROM ubuntu:latest

WORKDIR /app

EXPOSE 8080

COPY out/build/Linux-x64/docker/Gaboot ./

COPY out/build/Linux-x64/docker/config.json ./

CMD ["./Gaboot"]
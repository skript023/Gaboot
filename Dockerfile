FROM ubuntu:latest

WORKDIR /app

EXPOSE 8088

COPY out/build/Linux-x64/Gaboot ./

COPY out/build/Linux-x64/config.json ./

COPY out/build/Linux-x64/.env ./

CMD ["./Gaboot"]
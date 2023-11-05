FROM ubuntu:latest

WORKDIR /app

EXPOSE 8088

COPY build/Gaboot ./

COPY build/config.json ./

CMD ["./Gaboot"]
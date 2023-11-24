FROM ubuntu:latest

WORKDIR /app

EXPOSE 8080

COPY out/build/Linux-x64/Gaboot ./

COPY out/build/Linux-x64/config.json ./

COPY out/build/Linux-x64/.env ./

COPY out/build/Linux-x64/uploads/ ./uploads

CMD ["./Gaboot"]
FROM ubuntu:latest

WORKDIR /var/www

EXPOSE 8080

COPY out/build/Linux-x64/Gaboot ./

COPY out/build/Linux-x64/config.json ./

COPY out/build/Linux-x64/.env ./

COPY out/build/Linux-x64/uploads/ ./uploads

CMD ["sudo chmod 777 /var/www/Gaboot -R"]

CMD ["./Gaboot"]
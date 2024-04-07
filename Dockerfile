FROM ubuntu:latest

# Install Supervisor
RUN apt-get update && apt-get install -y supervisor curl

WORKDIR /home/skript023/gbt-backend/Linux

# Copy the necessary files
COPY Gaboot .
COPY config.json .
COPY ./assets ./assets

# Set permissions
RUN chmod +x /home/skript023/gbt-backend/Linux/Gaboot

# Copy Supervisor configuration file
COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

# Start Supervisor
CMD ["/usr/bin/supervisord", "-c", "/etc/supervisor/conf.d/supervisord.conf"]

EXPOSE 8080

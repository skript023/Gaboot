FROM mcr.microsoft.com/windows/servercore:ltsc2019

# Install Supervisor (You may need to find alternative for supervisor on Windows)
# For example, NSSM (Non-Sucking Service Manager) can be used to run processes as Windows services.
# Download NSSM from: https://nssm.cc/download
# Instructions for NSSM: https://nssm.cc/usage
# For this example, I'm just commenting out the supervisor-related lines.
# RUN apt-get update && apt-get install -y supervisor

WORKDIR C:\gaboot

# Copy the necessary files
COPY out\build\Windows-x64\Gaboot.exe .
COPY out\build\Windows-x64\config.json .
COPY out\build\Windows-x64\.env .
COPY out\build\Windows-x64\uploads\ ./uploads

# Set permissions (Not needed on Windows)
# RUN chmod +x /usr/local/bin/gaboot/Gaboot

# Copy Supervisor configuration file (Commenting out as supervisor is not used)
# COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

# Start Supervisor (Commenting out as supervisor is not used)
# CMD ["/usr/bin/supervisord", "-c", "/etc/supervisor/conf.d/supervisord.conf"]

# Example command to run the application directly (change as needed)
CMD ["Gaboot.exe"]

# Expose the necessary port (adjust as needed)
EXPOSE 8080

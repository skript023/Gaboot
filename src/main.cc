#include <drogon/drogon.h>

int main() 
{
    //Set HTTP listener address and port
    LOG_INFO << "Starting C++ API...";
    ADD_LISTENER("0.0.0.0", 8080);
    LOG_INFO << "Listener initialized.";
    LOG_INFO << "Load configuration...";
    ADD_CONFIG("./config.json");
    LOG_INFO << "Configuration applied.";
    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    LOG_INFO << "Server is running...";
    SERVER_RUN;

    return 0;
}

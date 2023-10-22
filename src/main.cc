#include <pch.h>

#include "util/file_manager.hpp"

int main() 
{
    using namespace gaboot;
    //Set base directory to file manager
    LOG_INFO << UPLOAD_PATH;
    g_file_manager.init(UPLOAD_PATH);
    //Set HTTP listener address and port
    LOG_INFO << "Starting C++ API...";
    ADD_LISTENER("127.0.0.1", 8080);
    LOG_INFO << "Listener initialized.";
    LOG_INFO << "Load configuration...";
    ADD_CONFIG("./config.json");
    LOG_INFO << "Configuration applied.";
    //Run HTTP framework,the method will block in the internal event loop
    LOG_INFO << "Server is running...";

    SERVER_RUN;
    
    LOG_INFO << "Server has ended...";

    return 0;
}

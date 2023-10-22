#include <pch.h>

#include "util/file_manager.hpp"

int main() 
{
    using namespace gaboot;
    //Set base directory to file manager
    g_file_manager.init(UPLOAD_PATH);

    auto logger_instance = std::make_unique<logger>("Drogon");

    //Set HTTP listener address and port
    LOG(INFO) << "Starting C++ Gaboot Server.";

    ADD_LISTENER("127.0.0.1", 8080);
    LOG(INFO) << "Listener initialized.";

    ADD_CONFIG("./config.json");
    LOG(INFO) << "Configuration loaded.";

    //Run HTTP framework,the method will block in the internal event loop
    LOG(INFO) << "Server has started.";
    SERVER_RUN;
    
    LOG(INFO) << "Server has shutdown.";

    logger_instance.reset();
    return 0;
}

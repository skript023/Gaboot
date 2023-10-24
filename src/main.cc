#include <pch.h>

#include "util/file_manager.hpp"

int main() 
{
    using namespace gaboot;
    //Set base directory to file manager
    auto logger_instance = std::make_unique<logger>("Gaboot Logger");

    LOG(RAW_GREEN_TO_CONSOLE) << R"kek(
  _____                            _____                   _             
 / ____|                          |  __ \                 (_)            
| (___   ___ _ ____   _____ _ __  | |__) |   _ _ __  _ __  _ _ __   __ _ 
 \___ \ / _ \ '__\ \ / / _ \ '__| |  _  / | | | '_ \| '_ \| | '_ \ / _` |
 ____) |  __/ |   \ V /  __/ |    | | \ \ |_| | | | | | | | | | | | (_| |
|_____/ \___|_|    \_/ \___|_|    |_|  \_\__,_|_| |_|_| |_|_|_| |_|\__, |
                                                                    __/ |
                                                                   |___/ 
)kek";

    g_file_manager.init(UPLOAD_PATH);

    //Set HTTP listener address and port
    LOG(INFO) << "Starting C++ Gaboot Server.";

    ADD_LISTENER("0.0.0.0", 8088);
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

#include <pch.h>

#include "util/file_manager.hpp"

int main() 
{
    using namespace gaboot;
    //Set base directory to file manager
    auto logger_instance = std::make_unique<logger>("Gaboot Logger");

    LOG(RAW_GREEN_TO_CONSOLE) << R"kek(
  _____       _                 _      _____                          
 / ____|     | |               | |    / ____|                         
| |  __  __ _| |__   ___   ___ | |_  | (___   ___ _ ____   _____ _ __ 
| | |_ |/ _` | '_ \ / _ \ / _ \| __|  \___ \ / _ \ '__\ \ / / _ \ '__|
| |__| | (_| | |_) | (_) | (_) | |_   ____) |  __/ |   \ V /  __/ |   
 \_____|\__,_|_.__/ \___/ \___/ \__| |_____/ \___|_|    \_/ \___|_|   
)kek";

    LOG(INFO) << "Upload folder created in " << UPLOAD_PATH;

    g_file_manager.init(UPLOAD_PATH);

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

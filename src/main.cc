#include <pch.h>

#include "util/file_manager.hpp"

int main() 
{
    using namespace gaboot;
    //Set base directory to file manager
    g_file_manager.init(UPLOAD_PATH);

    auto logger_instance = std::make_unique<logger>("Gaboot Logger");

    LOG(RAW_GREEN_TO_CONSOLE) << R"kek(
  _____       _                 _              _____ _____ 
 / ____|     | |               | |       /\   |  __ \_   _|
| |  __  __ _| |__   ___   ___ | |_     /  \  | |__) || |  
| | |_ |/ _` | '_ \ / _ \ / _ \| __|   / /\ \ |  ___/ | |  
| |__| | (_| | |_) | (_) | (_) | |_   / ____ \| |    _| |_ 
 \_____|\__,_|_.__/ \___/ \___/ \__| /_/    \_\_|   |_____|
)kek";

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

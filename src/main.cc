#include <pch.h>

#include "util/thread.hpp" 
#include "util/version.hpp"
#include "util/file_manager.hpp"
#include "scheduler/schedule.hpp"

#include "module/customer/services/customer_manager.hpp"
#include <scheduler/tasks.hpp>

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

    LOG(INFO) << fmt::format("\n\tGit Info\n\tBranch:\t{}\n\tHash:\t{}\n\tDate:\t{}", server_version::GIT_BRANCH, server_version::GIT_SHA1, server_version::GIT_DATE);

    LOG(INFO) << "Admin middleware is " << ADMIN_MIDDLEWARE_STATUS;
    LOG(INFO) << "Customer middleware is " << CUSTOMER_MIDDLEWARE_STATUS;

    g_file_manager.init(UPLOAD_PATH);

    //Set HTTP listener address and port
    LOG(INFO) << "Starting C++ Gaboot Server.";

    auto schedule_instance = std::make_unique<schedule>();
    auto tasks_instance = std::make_unique<tasks>();
    LOG(INFO) << "Schedule task initialized.";

    auto customer_manager_instance = std::make_unique<customer_manager>();
    LOG(INFO) << "Customer manager initialized.";

    auto thread_instance = std::make_unique<thread_pool>();
    LOG(INFO) << "Thread pool initialized.";

    ADD_LISTENER("0.0.0.0", 8088);
    LOG(INFO) << "Listener initialized.";
    ADD_CONFIG("./config.json");
    LOG(INFO) << "Configuration loaded.";

    //Run HTTP framework,the method will block in the internal event loop
    LOG(INFO) << "Server has started.";
    SERVER_RUN;
    
    LOG(INFO) << "Server has shutdown.";

    thread_instance->destroy();
    thread_instance.reset();
    LOG(INFO) << "Thread pool uninitialized.";

    customer_manager_instance.reset();
    LOG(INFO) << "Customer manager uninitialized.";

    tasks_instance.reset();
    schedule_instance.reset();
    LOG(INFO) << "Schedule task uninitialized.";

    logger_instance.reset();

    return EXIT_SUCCESS;
}

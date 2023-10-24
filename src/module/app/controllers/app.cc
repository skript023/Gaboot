#include "app.h"

// Add definition of your processing function here
namespace gaboot
{
	void app::main(HttpRequestPtr const& req, response_t&& callback)
	{
		auto response = HttpResponse::newHttpResponse();
		response->setBody("Hello World!");

		return callback(response);
	}
}
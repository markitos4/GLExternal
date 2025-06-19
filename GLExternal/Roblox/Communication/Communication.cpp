#include "Communication.hpp"
#include "HttpLibrary.h"
#include "nlohmann/json.hpp"

void CBridge::Initialize()
{
	std::thread([] {

		httplib::Server Bridge;

		Bridge.Post("/GLExecutor_ExecuteRequest", [](httplib::Request request, httplib::Response response) {
			try {
				const std::string Script = request.body;
				if (!Script.empty())
					Execute(Script);
			}
			catch (const std::exception& ex)
			{
				Logger()->Error("Error receiving and executing script: " + std::string(ex.what()));
				return;
			}
			});


		Bridge.listen("localhost", 7646);

	}).detach();
}

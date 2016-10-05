#include "stdafx.h"
#include "Application.h"

#ifndef TEST_MODE
int main()
{
	Logging::init();

	std::shared_ptr<Application> app = std::make_shared<Application>();

	LOG_INFO("Starting application...");

	app->init();
	app->run();

    return 0;
}
#endif

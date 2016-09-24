#include "stdafx.h"
#include "WorldOfCubes.h"

int main()
{
	Logging::init();

	WorldOfCubes app;

	LOG_INFO("Starting application...");

	app.init();
	app.run();

    return 0;
}
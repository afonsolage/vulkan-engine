#include "stdafx.h"
#include "WorldOfCubes.h"
#include "GameEngine.h"

WorldOfCubes::WorldOfCubes()
{

}


WorldOfCubes::~WorldOfCubes()
{

	printf("Application destroyed.\n");
}

void WorldOfCubes::init()
{
	try 
	{
		m_engine = std::make_shared<GameEngine>(m_app_name, m_app_version);
		m_engine->init();
		printf("Game Engine initialization sucessfull!\n");
	}
	catch (std::exception e)
	{
		printf("Exception Caught: %s\n", e.what());
	}
}

void WorldOfCubes::run()
{
	printf("Starting main loop!\n");
	while(m_engine->is_running())
	{
		m_engine->tick();
	}
}

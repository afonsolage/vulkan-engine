#include "stdafx.h"
#include "WorldOfCubes.h"
#include "GameEngine.h"

WorldOfCubes::WorldOfCubes()
{

}


WorldOfCubes::~WorldOfCubes()
{

	LOG_DEBUG("Application destroyed.");
}

void WorldOfCubes::init()
{
	try 
	{
		m_engine = std::make_shared<GameEngine>(m_app_name, m_app_version);
		m_engine->init();
		LOG_INFO("Game Engine initialization sucessfull!");
	}
	catch (std::exception e)
	{
		LOG_FATAL("Exception Caught: %s", e.what());
	}
}

void WorldOfCubes::run()
{
	LOG_INFO("Starting main loop!");
	while(m_engine->is_running())
	{
		m_engine->tick();
	}
}

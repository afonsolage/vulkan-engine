#include "stdafx.h"
#include "Application.h"
#include "GameEngine.h"
#include "WorldApplicationState.h"


Application::Application()
{

}


Application::~Application()
{
	if (m_state)
		m_state->terminate();

	LOG_DEBUG("Application destroyed.");
}

void Application::init()
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

void Application::run()
{
	LOG_INFO("Starting main loop!");
	while(m_engine->is_running())
	{
		if (m_state)
		{
			m_state->update(0.0f);
		}

		m_engine->tick();
	}
}
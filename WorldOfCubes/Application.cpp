#include "stdafx.h"
#include "Application.h"
#include "GameEngine.h"
#include "WorldApplicationState.h"
#include "GraphicsSystem.h"

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

		m_graphic_system = m_engine->get_graphics_sytem();
		LOG_INFO("Game Engine initialization sucessfull!");

		change_state<WorldApplicationState>();
	}
	catch (std::exception e)
	{
		LOG_FATAL("Exception Caught: %s", e.what());
	}
}

void Application::run()
{
	LOG_INFO("Starting main loop!");

	while (m_engine->is_running())
	{
		m_engine->tick();

		if (m_state)
		{
			m_state->update(0.0f);

			auto graphics = m_graphic_system.lock();

			if (!graphics)
			{
				LOG_FATAL("Failed to run update loop! Graphics engine is null!");
				throw std::runtime_error("Failed to run update loop! Graphics engine is null!");
			}
			graphics->render_scene(m_state->get_main_scene());
		}
	}
}
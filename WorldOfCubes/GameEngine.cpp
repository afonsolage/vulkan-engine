#include "stdafx.h"
#include "GameEngine.h"
#include "WindowSystem.h"
#include "GraphicsSystem.h"
#include "FileSystem.h"

GameEngine::GameEngine(std::string app_name, uint32_t app_version)
	: m_app_name(app_name)
	, m_app_version(app_version)
	, m_window_size({ 800, 600 })
{
	m_window_system = std::make_shared<WindowSystem>();
	m_file_system = std::make_shared<FileSystem>();
}


GameEngine::~GameEngine()
{
	printf("Game engine destroyed.\n");
}

void GameEngine::init()
{
	std::stringstream app_name;

	app_name << m_app_name.c_str() << " v" << VERSION_MAJOR(m_app_version) << "." << VERSION_MINOR(m_app_version) << "." << VERSION_PATCH(m_app_version);

	//TODO: Add window size settings;
	m_window_system->init(app_name.str().c_str(), m_window_size.x, m_window_size.y);

	m_graphic_system = std::make_shared<GraphicsSystem>(shared_from_this());
	m_graphic_system->init();
}

void GameEngine::tick()
{
	m_window_system->tick();
}

bool GameEngine::is_running()
{
	return !m_window_system->close_requested();
}

#pragma once

#include "AbstractApplicationState.h"

//Forwarding declarations
class GameEngine;

class Application : public std::enable_shared_from_this<Application>
{
public:
	Application();

	virtual ~Application();

	void init();
	void run();

	template<typename T>
	void change_state();

	std::weak_ptr<GameEngine> get_engine() const noexcept { return m_engine; }

private:
	std::shared_ptr<GameEngine> m_engine;

	const std::string m_app_name = "World of Cubes";
	const uint32_t m_app_version = MAKE_VERSION(0, 1, 0);

	std::unique_ptr<AbstractApplicationState> m_state;
};
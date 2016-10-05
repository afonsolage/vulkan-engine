#pragma once

#include "AbstractApplicationState.h"

//Forwarding declarations
class GameEngine;
class GraphicsSystem;

class Application : public std::enable_shared_from_this<Application>
{
public:
	Application();

	virtual ~Application();

	void init();
	void run();

	template<typename T>
	void change_state();

	template<typename T>
	bool is_state();

	std::weak_ptr<GameEngine> get_engine() const noexcept { return m_engine; }

private:
	std::shared_ptr<GameEngine> m_engine;
	std::weak_ptr<GraphicsSystem> m_graphic_system;

	const std::string m_app_name = "World of Cubes";
	const uint32_t m_app_version = MAKE_VERSION(0, 1, 0);

	std::unique_ptr<AbstractApplicationState> m_state;
};

template<typename T>
void Application::change_state()
{
	if (m_state)
		m_state->terminate();

	m_state = std::make_unique<T>(shared_from_this());
	m_state->initialize();
}

template<typename T>
inline bool Application::is_state()
{
	auto& t = *m_state;
	return typeid(t) == typeid(T);
}

#pragma once

class Application;
class Camera;
class GameEngine;

class AbstractApplicationState
{
public:
	AbstractApplicationState(std::shared_ptr<Application> application);
	AbstractApplicationState(const AbstractApplicationState& other) = delete;

	virtual ~AbstractApplicationState();

	void initialize();
	void update(float delta);
	void terminate();

	bool is_terminated() const noexcept { return m_terminated; }

protected:
	virtual void on_initialized() = 0;
	virtual void on_updated(float delta) = 0;
	virtual void on_terminated() = 0;

	std::weak_ptr<Application> m_application;
	std::weak_ptr<GameEngine> m_engine;

	bool m_terminated = false;
};
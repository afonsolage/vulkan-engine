#pragma once

class Application;
class CameraComponent;
class GameEngine;
class Scene;

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

	std::shared_ptr<Scene>& get_main_scene() { return m_main_scene; }

protected:
	virtual void on_initialized() = 0;
	virtual void on_updated(float delta) = 0;
	virtual void on_terminated() = 0;

	std::shared_ptr<Scene> m_main_scene;

	std::weak_ptr<Application> m_application;
	std::weak_ptr<GameEngine> m_engine;

	bool m_terminated = false;
};
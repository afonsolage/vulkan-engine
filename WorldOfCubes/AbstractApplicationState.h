#pragma once

class Application;
class Camera;

class AbstractApplicationState
{
public:
	AbstractApplicationState(std::shared_ptr<Application> application);
	virtual ~AbstractApplicationState();

	virtual void initialize();
	virtual void update(float delta);
	virtual void terminate();

	virtual std::weak_ptr<Camera> get_main_camera() const noexcept { return m_main_camera; }
	virtual bool is_terminated() const noexcept { return m_terminated; }

protected:

	std::weak_ptr<Application> m_application;
	std::shared_ptr<Camera> m_main_camera;

	bool m_terminated = false;
};
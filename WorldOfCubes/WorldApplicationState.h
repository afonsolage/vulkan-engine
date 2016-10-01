#pragma once

#include "AbstractApplicationState.h"

class Camera;
class GameEngine;
class GraphicsSystem;

class WorldApplicationState :
	public AbstractApplicationState
{
public:
	WorldApplicationState(std::shared_ptr<Application> application);
	virtual ~WorldApplicationState();

private:
	virtual void on_initialized() override;
	virtual void on_updated(float delta) override;
	virtual void on_terminated() override;

	std::shared_ptr<Camera> m_main_camera;
	std::weak_ptr<GraphicsSystem> m_graphics_system;
};


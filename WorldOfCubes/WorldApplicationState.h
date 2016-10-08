#pragma once

#include "AbstractApplicationState.h"

class CameraComponent;
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

	void setup_camera();
	void create_basic_triangle();

	std::shared_ptr<CameraComponent> m_main_camera;
	std::weak_ptr<GraphicsSystem> m_graphics_system;
};

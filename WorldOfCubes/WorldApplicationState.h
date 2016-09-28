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
	virtual void initialize() override;
	virtual void update(float delta) override;
	virtual void terminate() override;

	std::shared_ptr<Camera> m_main_camera;

	std::weak_ptr<GameEngine> m_engine;
	std::weak_ptr<GraphicsSystem> m_graphics_system;
};


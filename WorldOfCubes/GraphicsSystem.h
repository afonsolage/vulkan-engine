#pragma once

//Forwarding declarations
class GameEngine;
class Context;
class Swapchain;
class GraphicsPipeline;
class ShaderManager;

class GraphicsSystem : public std::enable_shared_from_this<GraphicsSystem>
{
public:
	GraphicsSystem(std::shared_ptr<GameEngine>& engine);

	virtual ~GraphicsSystem();

	void init();

	std::weak_ptr<Context> get_context() { return m_context; }
	std::weak_ptr<Swapchain> get_swapchain() { return m_swapchain; }
	std::weak_ptr<GameEngine> get_engine() { return m_engine; }
	std::weak_ptr<ShaderManager> get_shader_manager() { return m_shader_manager; }

	uint32_t get_window_width() const;
	uint32_t get_window_height() const;

private:

	//Owned objects
	std::shared_ptr<Context> m_context;
	std::shared_ptr<ShaderManager> m_shader_manager;
	std::shared_ptr<Swapchain> m_swapchain;
	std::shared_ptr<GraphicsPipeline> m_pipeline;

	//Non-owner objects
	std::weak_ptr<GameEngine> m_engine;

};


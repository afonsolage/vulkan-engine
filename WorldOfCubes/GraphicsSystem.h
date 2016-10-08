#pragma once

//Forwarding declarations
class GameEngine;
class Context;
class Swapchain;
class BaseGraphicsPipeline;
class ShaderSystem;
class type_info;
class Scene;
class AbstractMaterial;

class GraphicsSystem : public std::enable_shared_from_this<GraphicsSystem>
{
public:
	GraphicsSystem(std::shared_ptr<GameEngine>& engine);

	virtual ~GraphicsSystem();

	void init();
	void init_materials();

	std::weak_ptr<Context> get_context() { return m_context; }
	std::weak_ptr<Swapchain> get_swapchain() { return m_swapchain; }
	std::weak_ptr<GameEngine> get_engine() { return m_engine; }
	std::weak_ptr<ShaderSystem> get_shader_system() { return m_shader_manager; }
	std::weak_ptr<BaseGraphicsPipeline> get_base_pipeline() { return m_base_pipeline; }

	template<typename T>
	std::shared_ptr<AbstractMaterial>& get_material();

	uint32_t get_window_width() const;
	uint32_t get_window_height() const;

	void render_scene(std::shared_ptr<Scene>& scene);
private:

	//Owned objects
	std::shared_ptr<Context> m_context;
	std::shared_ptr<ShaderSystem> m_shader_manager;
	std::shared_ptr<Swapchain> m_swapchain;
	std::shared_ptr<BaseGraphicsPipeline> m_base_pipeline;

	//Non-owner objects
	std::weak_ptr<GameEngine> m_engine;

	std::unordered_map<const type_info*, std::shared_ptr<const AbstractMaterial>> m_material_map;
};

template<typename T>
inline std::shared_ptr<AbstractMaterial>& GraphicsSystem::get_material()
{
	auto it = m_material_map.find(&typeid(T));

	if (it == m_material_map.end())
	{
		LOG_FATAL("Failed to find material: %s.", typeid(T).name());
		throw std::runtime_error("No such material!");
	}
	else
	{
		return it->second;
	}
}

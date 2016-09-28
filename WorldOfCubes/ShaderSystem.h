#pragma once

#include "VulkanAPI.h"

class GraphicsSystem;
class FileSystem;
class Context;

struct ShaderInfo
{
	std::string name;
	std::string entry;

	vk::ShaderStageFlagBits type;
	vk::ShaderModule module;

	inline bool is_loaded() const noexcept
	{
		return (bool)module;
	}

	ShaderInfo(std::string&& name, vk::ShaderStageFlagBits&& type, std::string&& entry) : name(name), type(type), entry(entry) {}
	ShaderInfo(const std::string& name, const vk::ShaderStageFlagBits& type, const std::string& entry) : name(name), type(type), entry(entry) {}

	ShaderInfo(ShaderInfo&& other) : name(std::move(other.name)), type(other.type), entry(std::move(other.entry)) {}
	ShaderInfo(const ShaderInfo& other) : name(other.name), type(other.type), entry(other.entry) {}
};

class ShaderSystem
{
public:

	enum class Shader
	{
		eNone, //THIS ONE SHOULD BE THE FIRST ONE. ALWAYS.
		eColoredVert,
		eColoredFrag,

		eMax, //THIS ONE SHOULD BE THE LAST ONE. ALWAYS.
	};

	ShaderSystem(std::shared_ptr<GraphicsSystem>& graphics_system);
	~ShaderSystem();

	void init();

	std::vector<vk::PipelineShaderStageCreateInfo> get_shader_create_info(std::vector<ShaderSystem::Shader> shaders);


private:
	const ShaderInfo& get_shader_info(ShaderSystem::Shader shader, bool load_module = true);

	void load_shader_module(ShaderInfo& shader_info);

	std::weak_ptr<GraphicsSystem> m_graphics_system;
	std::weak_ptr<FileSystem> m_file_system;
	std::weak_ptr<Context> m_context;

	std::unordered_map<ShaderSystem::Shader, ShaderInfo> m_shader_module_map;

	const std::string m_shader_folder = "assets/shader";
	const std::string m_shader_info_file_path = "assets/shader/shader.info";
	const std::string m_shader_extension = ".spv";

	const std::string m_shader_info_name = "name";
	const std::string m_shader_info_type = "type";
	const std::string m_shader_info_entry = "entry";

	const std::unordered_map<std::string, vk::ShaderStageFlagBits> m_shader_type_map =
	{
		{ "vertex", vk::ShaderStageFlagBits::eVertex },
		{ "fragment", vk::ShaderStageFlagBits::eFragment },
	};
};


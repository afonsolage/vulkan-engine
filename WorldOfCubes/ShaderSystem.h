#pragma once

#include "VulkanAPI.h"
#include "MeshBuffer.h"
#include "boost/optional.hpp"
#include "FileSystem.h"

class GraphicsSystem;
class FileSystem;
class Context;

struct ShaderInfo
{
	std::string m_name;
	std::string m_entry;

	vk::ShaderStageFlagBits m_type;
	vk::ShaderModule m_module;

	MeshBuffer m_buffer;

	inline bool is_loaded() const noexcept
	{
		return (bool)m_module;
	}
	inline bool is_valid() const noexcept
	{
		return !m_name.empty();
	}
	inline bool is_buffer_available() const noexcept
	{
		return m_buffer.is_initialized();
	}

	inline vk::PipelineShaderStageCreateInfo get_shader_create_info() const noexcept
	{
		if (!is_loaded())
		{
			LOG_WARN("Trying to generate a shader stage create info from a non-loaded shader.");
		}
		return{ vk::PipelineShaderStageCreateFlags(), m_type, m_module, m_entry.c_str() };
	}

	ShaderInfo() {};

	ShaderInfo(std::string&& name, vk::ShaderStageFlagBits&& type, std::string&& entry) : m_name(name), m_type(type), m_entry(entry) {}
	ShaderInfo(const std::string& name, const vk::ShaderStageFlagBits& type, const std::string& entry) : m_name(name), m_type(type), m_entry(entry) {}

	ShaderInfo(ShaderInfo&& other) : m_name(std::move(other.m_name)), m_type(other.m_type), m_entry(std::move(other.m_entry)) {}
	ShaderInfo(const ShaderInfo& other) : m_name(other.m_name), m_type(other.m_type), m_entry(other.m_entry) {}
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

	std::shared_ptr<ShaderInfo>& get_shader_info(ShaderSystem::Shader shader, bool load_module = true);

	static std::vector<vk::VertexInputAttributeDescription> get_mesh_buffer_attribute_description(const MeshBuffer& buffer);
	static vk::VertexInputBindingDescription get_mesh_buffer_binding_description(const MeshBuffer& buffer);

private:
	static vk::Format get_element_format(const MeshBuffer::MeshBufferElement& element);

	void load_shader_module(ShaderInfo& shader_info);

	std::weak_ptr<GraphicsSystem> m_graphics_system;
	std::weak_ptr<FileSystem> m_file_system;
	std::weak_ptr<Context> m_context;

	std::unordered_map<ShaderSystem::Shader, std::shared_ptr<ShaderInfo>> m_shader_module_map;

	void parse_shader_input(ShaderInfo& shader, const ptree& input);

	const std::string m_shader_folder = "assets/shader";
	const std::string m_shader_info_file_path = "assets/shader/shader.info";
	const std::string m_shader_extension = ".spv";

	const std::string m_shader_info_name = "name";
	const std::string m_shader_info_type = "type";
	const std::string m_shader_info_entry = "entry";

	const std::string m_shader_info_input = "input";
	const std::string m_shader_info_input_float = "float";
	const std::string m_shader_info_input_uint = "uint";
	const std::string m_shader_info_input_int = "int";

	const std::unordered_map<std::string, vk::ShaderStageFlagBits> m_shader_type_map =
	{
		{ "vertex", vk::ShaderStageFlagBits::eVertex },
		{ "fragment", vk::ShaderStageFlagBits::eFragment },
	};
};
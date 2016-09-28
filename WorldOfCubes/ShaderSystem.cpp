#include "stdafx.h"
#include "ShaderSystem.h"

#include "GraphicsSystem.h"
#include "GameEngine.h"
#include "FileSystem.h"
#include "Context.h"

ShaderSystem::ShaderSystem(std::shared_ptr<GraphicsSystem>& graphics_system)
	: m_graphics_system(graphics_system)
{
	m_context = graphics_system->get_context();
}

ShaderSystem::~ShaderSystem()
{
	GET_CONTEXT;
	for (const auto& shader : m_shader_module_map)
	{
		if (shader.second.is_loaded())
		{
			context->m_device.destroyShaderModule(shader.second.module);
		}
	}

	LOG_DEBUG("Shader manager destroyed.");
}

void ShaderSystem::init()
{
	SAFE_GET(graphics_system, m_graphics_system);
	SAFE_GET(game_engine, graphics_system->get_engine());
	SAFE_GET(file_system, game_engine->get_file_system());

	m_file_system = file_system;

	ptree shader_info = file_system->load_info_file(m_shader_info_file_path);

	auto begin_shader_type = ((uint32_t)ShaderSystem::Shader::eNone) + 1;
	auto end_shader_type = (uint32_t)ShaderSystem::Shader::eMax;

	for (auto i = begin_shader_type; i < end_shader_type; i++)
	{
		auto shader_opt = shader_info.get_child_optional(std::to_string(i));

		if (!shader_opt.is_initialized())
		{
			LOG_WARN("Shader type %d not found on shader info file!", i);
			continue;
		}

		auto shader = shader_opt.get();

		auto name_opt = shader.get_optional<std::string>(m_shader_info_name);

		if (!name_opt.is_initialized())
		{
			LOG_ERROR("Info name not found on shader %d.", i);
			continue;
		}

		auto type_opt = shader.get_optional<std::string>(m_shader_info_type);

		if (!type_opt.is_initialized())
		{
			LOG_ERROR("Info type not found on shader %d.", i);
			continue;
		}

		auto type_it = m_shader_type_map.find(type_opt.get());

		if (type_it == m_shader_type_map.end())
		{
			LOG_ERROR("Info type invalid on shader %d.", i);
			continue;
		}

		auto entry_opt = shader.get_optional<std::string>(m_shader_info_entry);

		if (!entry_opt.is_initialized())
		{
			LOG_ERROR("Info entry not found on shader %d.", i);
			continue;
		}

		m_shader_module_map.emplace(
			std::piecewise_construct,
			std::forward_as_tuple((ShaderSystem::Shader)i),
			std::forward_as_tuple(std::move(name_opt.get()), type_it->second, std::move(entry_opt.get()))
		);
	}

	LOG_INFO("Shader info load completed. %d shaders was loaded.", m_shader_module_map.size());
}

std::vector<vk::PipelineShaderStageCreateInfo> ShaderSystem::get_shader_create_info(std::vector<ShaderSystem::Shader> shaders)
{
	std::vector<vk::PipelineShaderStageCreateInfo> result(shaders.size(), vk::PipelineShaderStageCreateInfo());

	uint32_t i = 0;
	for (const auto& shader : shaders)
	{
		const auto& shader_info = get_shader_info(shader);

		result[i].stage = shader_info.type;
		result[i].module = shader_info.module;
		result[i].pName = shader_info.entry.c_str();

		i++;
	}

	return result;
}

const ShaderInfo& ShaderSystem::get_shader_info(ShaderSystem::Shader shader, bool load_module)
{
	auto it = m_shader_module_map.find(shader);

	if (it == m_shader_module_map.end())
	{
		throw std::runtime_error("Shader info not loaded!");
	}

	auto& shader_info = it->second;

	if (load_module && !shader_info.is_loaded())
	{
		load_shader_module(shader_info);
	}

	return shader_info;
}

void ShaderSystem::load_shader_module(ShaderInfo& shader_info)
{
	std::stringstream ss;

	ss << m_shader_folder << "/" << shader_info.name << m_shader_extension;

	SAFE_GET(file_system, m_file_system);

	auto file_content = file_system->read_file(ss.str());

	if (file_content.empty())
	{
		ss.clear();
		ss << "Failed to load shader module " << shader_info.name << "." << std::endl;
		throw std::runtime_error(ss.str());
	}

	vk::ShaderModuleCreateInfo create_info;
	create_info.codeSize = file_content.size();
	create_info.pCode = (uint32_t*)file_content.data();


	GET_CONTEXT;
	shader_info.module = context->m_device.createShaderModule(create_info);
}
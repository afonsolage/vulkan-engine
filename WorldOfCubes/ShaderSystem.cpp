#include "stdafx.h"
#include "ShaderSystem.h"

#include "GraphicsSystem.h"
#include "GameEngine.h"
#include "Context.h"

ShaderSystem::ShaderSystem(std::shared_ptr<GraphicsSystem>& graphics_system)
	: m_graphics_system(graphics_system)
{
	m_context = graphics_system->get_context();
}

ShaderSystem::~ShaderSystem()
{
	auto context = m_context.lock();
	if (context)
	{
		for (const auto& shader : m_shader_module_map)
		{
			if (shader.second && shader.second->is_loaded())
			{
				context->m_device.destroyShaderModule(shader.second->m_module);
			}
		}
		LOG_DEBUG("Shader manager destroyed.");
	}
	else
	{
		LOG_FATAL("Unable to destroy ShaderSystem: Context is null.");
	}
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

		auto shader_info = std::make_shared<ShaderInfo>(std::move(name_opt.get()), type_it->second, std::move(entry_opt.get()));

		m_shader_module_map.emplace(
			std::piecewise_construct,
			std::forward_as_tuple((ShaderSystem::Shader)i),
			std::forward_as_tuple(shader_info)
		);

		auto input_opt = shader.get_child_optional(m_shader_info_input);

		if (input_opt.is_initialized())
		{
			parse_shader_input(*shader_info, input_opt.get());
		}

	}

	LOG_INFO("Shader info load completed. %d shaders was loaded.", m_shader_module_map.size());
}

std::shared_ptr<ShaderInfo>& ShaderSystem::get_shader_info(ShaderSystem::Shader shader, bool load_module)
{
	auto it = m_shader_module_map.find(shader);

	if (it == m_shader_module_map.end() || !it->second)
	{
		throw std::runtime_error("Shader info not loaded!");
	}

	auto& shader_info = *it->second;

	if (load_module && !shader_info.is_loaded())
	{
		load_shader_module(shader_info);
	}

	return it->second;
}
std::vector<vk::VertexInputAttributeDescription> ShaderSystem::get_mesh_buffer_attribute_description(const MeshBuffer& buffer)
{
	std::vector<vk::VertexInputAttributeDescription> result;

	for (const auto& element : buffer)
	{
		result.emplace_back(element.m_location, 0, get_element_format(element), element.m_offset);
	}

	return result;
}

vk::VertexInputBindingDescription ShaderSystem::get_mesh_buffer_binding_description(const MeshBuffer& buffer)
{
	vk::VertexInputBindingDescription result;

	result.binding = 0;
	result.inputRate = vk::VertexInputRate::eVertex;
	result.stride = buffer.get_stride();

	return result;
}

vk::Format ShaderSystem::get_element_format(const MeshBuffer::MeshBufferElement & element)
{
	switch (element.m_format)
	{
	case MeshBufferElementFormat::eFloat:
	{
		switch (element.m_element_bits)
		{
		case MeshBufferElementBits::e8:
		{
			switch (element.m_element_count)
			{
			case MeshBufferElementCount::e1:
				return vk::Format::eR8Sint;
			case MeshBufferElementCount::e2:
				return vk::Format::eR8G8Sint;
			case MeshBufferElementCount::e3:
				return vk::Format::eR8G8B8Sint;
			case MeshBufferElementCount::e4:
				return vk::Format::eR8G8B8A8Sint;
			}
		}

		case MeshBufferElementBits::e16:
		{
			switch (element.m_element_count)
			{
			case MeshBufferElementCount::e1:
				return vk::Format::eR16Sfloat;

			case MeshBufferElementCount::e2:
				return vk::Format::eR16G16Sfloat;

			case MeshBufferElementCount::e3:
				return vk::Format::eR16G16B16Sfloat;

			case MeshBufferElementCount::e4:
				return vk::Format::eR16G16B16A16Sfloat;
			}
		}

		case MeshBufferElementBits::e32:
		{
			switch (element.m_element_count)
			{
			case MeshBufferElementCount::e1:
				return vk::Format::eR32Sfloat;
			case MeshBufferElementCount::e2:
				return vk::Format::eR32G32Sfloat;
			case MeshBufferElementCount::e3:
				return vk::Format::eR32G32B32Sfloat;
			case MeshBufferElementCount::e4:
				return vk::Format::eR32G32B32A32Sfloat;
			}
		}

		case MeshBufferElementBits::e64:
		{
			switch (element.m_element_count)
			{
			case MeshBufferElementCount::e1:
				return vk::Format::eR64Sfloat;
			case MeshBufferElementCount::e2:
				return vk::Format::eR64G64Sfloat;
			case MeshBufferElementCount::e3:
				return vk::Format::eR64G64B64Sfloat;
			case MeshBufferElementCount::e4:
				return vk::Format::eR64G64B64A64Sfloat;
			}
		}
		}
	}

	case MeshBufferElementFormat::eUnsignedInt:
	{
		switch (element.m_element_bits)
		{
		case MeshBufferElementBits::e8:
		{
			switch (element.m_element_count)
			{
			case MeshBufferElementCount::e1:
				return vk::Format::eR8Uint;
			case MeshBufferElementCount::e2:
				return vk::Format::eR8G8Uint;
			case MeshBufferElementCount::e3:
				return vk::Format::eR8G8B8Uint;
			case MeshBufferElementCount::e4:
				return vk::Format::eR8G8B8A8Uint;
			}
		}

		case MeshBufferElementBits::e16:
		{
			switch (element.m_element_count)
			{
			case MeshBufferElementCount::e1:
				return vk::Format::eR16Uint;
			case MeshBufferElementCount::e2:
				return vk::Format::eR16G16Uint;
			case MeshBufferElementCount::e3:
				return vk::Format::eR16G16B16Uint;
			case MeshBufferElementCount::e4:
				return vk::Format::eR16G16B16A16Uint;
			}
		}

		case MeshBufferElementBits::e32:
		{
			switch (element.m_element_count)
			{
			case MeshBufferElementCount::e1:
				return vk::Format::eR32Uint;
			case MeshBufferElementCount::e2:
				return vk::Format::eR32G32Uint;
			case MeshBufferElementCount::e3:
				return vk::Format::eR32G32B32Uint;
			case MeshBufferElementCount::e4:
				return vk::Format::eR32G32B32A32Uint;
			}
		}

		case MeshBufferElementBits::e64:
		{
			switch (element.m_element_count)
			{
			case MeshBufferElementCount::e1:
				return vk::Format::eR64Uint;
			case MeshBufferElementCount::e2:
				return vk::Format::eR64G64Uint;
			case MeshBufferElementCount::e3:
				return vk::Format::eR64G64B64Uint;
			case MeshBufferElementCount::e4:
				return vk::Format::eR64G64B64A64Uint;
			}
		}
		}
	}

	case MeshBufferElementFormat::eSignedInt:
	{
		switch (element.m_element_bits)
		{
		case MeshBufferElementBits::e8:
		{
			switch (element.m_element_count)
			{
			case MeshBufferElementCount::e1:
				return vk::Format::eR8Sint;
			case MeshBufferElementCount::e2:
				return vk::Format::eR8G8Sint;
			case MeshBufferElementCount::e3:
				return vk::Format::eR8G8B8Sint;
			case MeshBufferElementCount::e4:
				return vk::Format::eR8G8B8A8Sint;
			}
		}

		case MeshBufferElementBits::e16:
		{
			switch (element.m_element_count)
			{
			case MeshBufferElementCount::e1:
				return vk::Format::eR16Sint;
			case MeshBufferElementCount::e2:
				return vk::Format::eR16G16Sint;
			case MeshBufferElementCount::e3:
				return vk::Format::eR16G16B16Sint;
			case MeshBufferElementCount::e4:
				return vk::Format::eR16G16B16A16Sint;
			}
		}

		case MeshBufferElementBits::e32:
		{
			switch (element.m_element_count)
			{
			case MeshBufferElementCount::e1:
				return vk::Format::eR32Sint;
			case MeshBufferElementCount::e2:
				return vk::Format::eR32G32Sint;
			case MeshBufferElementCount::e3:
				return vk::Format::eR32G32B32Sint;
			case MeshBufferElementCount::e4:
				return vk::Format::eR32G32B32A32Sint;
			}
		}

		case MeshBufferElementBits::e64:
		{
			switch (element.m_element_count)
			{
			case MeshBufferElementCount::e1:
				return vk::Format::eR64Sint;
			case MeshBufferElementCount::e2:
				return vk::Format::eR64G64Sint;
			case MeshBufferElementCount::e3:
				return vk::Format::eR64G64B64Sint;
			case MeshBufferElementCount::e4:
				return vk::Format::eR64G64B64A64Sint;
			}
		}
		}
	}
	}

	throw std::runtime_error("Failed to cast Mesh Buffer element to Vulkan format.");
}

void ShaderSystem::load_shader_module(ShaderInfo& shader_info)
{
	std::stringstream ss;

	ss << m_shader_folder << "/" << shader_info.m_name << m_shader_extension;

	SAFE_GET(file_system, m_file_system);

	auto file_content = file_system->read_file(ss.str());

	if (file_content.empty())
	{
		ss.clear();
		ss << "Failed to load shader module " << shader_info.m_name << "." << std::endl;
		throw std::runtime_error(ss.str());
	}

	vk::ShaderModuleCreateInfo create_info;
	create_info.codeSize = file_content.size();
	create_info.pCode = (uint32_t*)file_content.data();


	GET_CONTEXT;
	shader_info.m_module = context->m_device.createShaderModule(create_info);
}

void ShaderSystem::parse_shader_input(ShaderInfo& shader, const ptree& input)
{
	uint32_t location = 0;
	char tmp[128] = {};
	std::vector<std::string> items;

	sprintf(tmp, "%d", location);
	auto info = input.get_optional<std::string>(tmp);

	std::vector<std::tuple<uint32_t, MeshBufferElementFormat, MeshBufferElementBits, MeshBufferElementCount>> tuples;

	while (info.is_initialized())
	{
		items.clear();

		auto str = info.get();

		std::transform(begin(str), end(str), begin(str), tolower);
		split(str, ' ', items);

		if (items.size() != 3)
		{
			LOG_FATAL("Invalid MeshBuffer parse: Insuficiente elements on input: %s", str.c_str());
			throw std::invalid_argument("Invalid input string");
		}

		MeshBufferElementFormat format;

		if (strncmp(items[0].c_str(), m_shader_info_input_float.c_str(), 5) == 0)
		{
			format = MeshBufferElementFormat::eFloat;
		}
		else if (strncmp(items[0].c_str(), m_shader_info_input_uint.c_str(), 4) == 0)
		{
			format = MeshBufferElementFormat::eUnsignedInt;
		}
		else if (strncmp(items[0].c_str(), m_shader_info_input_int.c_str(), 3) == 0)
		{
			format = MeshBufferElementFormat::eSignedInt;
		}
		else
		{
			LOG_FATAL("Invalid MeshBuffer parse: Unkown input format: %s", items[0].c_str());
			throw std::invalid_argument("Invalid input format string");
		}

		MeshBufferElementBits bits;

		if (strncmp(items[1].c_str(), "8", 1) == 0)
		{
			bits = MeshBufferElementBits::e8;
		}
		else if (strncmp(items[1].c_str(), "16", 2) == 0)
		{
			bits = MeshBufferElementBits::e16;
		}
		else if (strncmp(items[1].c_str(), "32", 2) == 0)
		{
			bits = MeshBufferElementBits::e32;
		}
		else if (strncmp(items[1].c_str(), "64", 2) == 0)
		{
			bits = MeshBufferElementBits::e64;
		}
		else
		{
			LOG_FATAL("Invalid MeshBuffer parse: Unkown input bits: %s", items[0].c_str());
			throw std::invalid_argument("Invalid input bits string");
		}

		MeshBufferElementCount count;

		if (strncmp(items[2].c_str(), "1", 1) == 0)
		{
			count = MeshBufferElementCount::e1;
		}
		else if (strncmp(items[2].c_str(), "2", 1) == 0)
		{
			count = MeshBufferElementCount::e2;
		}
		else if (strncmp(items[2].c_str(), "3", 1) == 0)
		{
			count = MeshBufferElementCount::e3;
		}
		else if (strncmp(items[2].c_str(), "4", 1) == 0)
		{
			count = MeshBufferElementCount::e4;
		}
		else
		{
			LOG_FATAL("Invalid MeshBuffer parse: Unkown input count: %s", items[0].c_str());
			throw std::invalid_argument("Invalid input count string");
		}

		tuples.emplace_back(location, format, bits, count);

		location++;
		sprintf(tmp, "%d", location);
		info = input.get_optional<std::string>(tmp);
	}

	std::sort(begin(tuples), end(tuples), [](const auto& t1, const auto& t2)
	{
		return std::get<uint32_t>(t1) < std::get<uint32_t>(t1);
	});

	for (const auto& t : tuples)
	{
		shader.m_buffer.add_buffer_element(std::get<MeshBufferElementFormat>(t), std::get<MeshBufferElementBits>(t), std::get<MeshBufferElementCount>(t));
	}

	shader.m_buffer.initialize();
}
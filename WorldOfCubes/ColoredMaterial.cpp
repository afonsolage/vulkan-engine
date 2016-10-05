#include "stdafx.h"
#include "ColoredMaterial.h"
#include "GraphicsSystem.h"
#include "Context.h"
#include "ShaderSystem.h"
#include "BaseGraphicsPipeline.h"

ColoredMaterial::ColoredMaterial(std::shared_ptr<GraphicsSystem> graphics_system)
	: AbstractMaterial(graphics_system)
{
	SAFE_GET(shader_system, m_shader_system);

	m_vertex_shader = shader_system->get_shader_info(ShaderSystem::Shader::eColoredVert);
	m_fragment_shader = shader_system->get_shader_info(ShaderSystem::Shader::eColoredFrag);
}

ColoredMaterial::~ColoredMaterial()
{
	LOG_DEBUG("Colored Material destroyed.");
}
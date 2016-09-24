#include "stdafx.h"
#include "GraphicsSystem.h"
#include "Context.h"
#include "GameEngine.h"
#include "Swapchain.h"
#include "GraphicsPipeline.h"
#include "ShaderManager.h"

GraphicsSystem::GraphicsSystem(std::shared_ptr<GameEngine>& engine)
	: m_engine(engine)
{
	m_context = std::make_shared<Context>(engine);
}

GraphicsSystem::~GraphicsSystem()
{
	LOG_DEBUG("Graphics system destroyed.");
}

void GraphicsSystem::init()
{
	m_context->init();

	m_shader_manager = std::make_shared<ShaderManager>(shared_from_this());
	m_shader_manager->init();

	m_swapchain = std::make_shared<Swapchain>(shared_from_this());
	m_swapchain->init();

	m_pipeline = std::make_shared<GraphicsPipeline>(shared_from_this());
	m_pipeline->init();
}

uint32_t GraphicsSystem::get_window_width() const
{
	auto engine = m_engine.lock();
	assert(engine);
	return engine->get_window_width();
}

uint32_t GraphicsSystem::get_window_height() const
{
	auto engine = m_engine.lock();
	assert(engine);
	return engine->get_window_height();
}

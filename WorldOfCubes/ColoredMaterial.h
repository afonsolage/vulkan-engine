#pragma once

#include "AbstractMaterial.h"
#include "MVPUniformBuffer.h"

struct ShaderInfo;

class ColoredMaterial : public AbstractMaterial
{
public:
	static const uint32_t POSITION_LOCATION = 0;
	static const uint32_t COLOR_LOCATION = 1;

	ColoredMaterial(std::shared_ptr<GraphicsSystem> graphics_system);
	virtual ~ColoredMaterial();

	virtual void init() override;
	virtual void pre_render(std::shared_ptr<CameraComponent>& camera) override;
	virtual void render(vk::CommandBuffer& cmd_buffer, std::shared_ptr<Entity>& mesh) override;

protected:
	virtual void create_descriptor_set_layout() override;
	virtual void create_descriptor_pool();
	virtual void create_buffers();

	MVPUniformBuffer m_mvp;

	vk::DescriptorPool m_descriptor_pool;

	//TODO: Add those buffers and device memory from shared states.
	vk::Buffer m_uniform_buffer;
	vk::Buffer m_staging_uniform_buffer;
};
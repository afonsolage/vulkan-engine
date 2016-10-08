#pragma once

#include "AbstractMaterial.h"

struct ShaderInfo;

class ColoredMaterial : public AbstractMaterial
{
public:
	static const uint32_t POSITION_LOCATION = 0;
	static const uint32_t COLOR_LOCATION = 1;

	ColoredMaterial(std::shared_ptr<GraphicsSystem> graphics_system);
	virtual ~ColoredMaterial();

	virtual void init() override;
	virtual void pre_render(std::shared_ptr<CameraComponent> camera) override;
	virtual void render(std::shared_ptr<MeshComponent> mesh) override;

protected:
	virtual void create_descriptor_set_layout() override;
	virtual void create_descriptor_pool();

	vk::DescriptorPool m_descriptor_pool;
};
#pragma once

#include "AbstractMaterial.h"

struct ShaderInfo;

class ColoredMaterial : public AbstractMaterial
{
public:
	ColoredMaterial(std::shared_ptr<GraphicsSystem> graphics_system);
	virtual ~ColoredMaterial();
};


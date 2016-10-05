#pragma once

#include "AbstractComponent.h"

class MeshComponent : public AbstractComponent
{

public:
	MeshComponent();
	virtual ~MeshComponent();

	void update() override;
	void on_attach() override;
	void on_detach() override;


};
#pragma once

#include "AbstractComponent.h"
#include "MeshBuffer.h"

class AbstractMaterial;

class MeshComponent : public AbstractComponent
{

public:
	MeshComponent();
	virtual ~MeshComponent();

	void update() override;
	void on_attach() override;
	void on_detach() override;

	void set_material(std::shared_ptr<const AbstractMaterial>& material);
	const std::vector<char>& get_buffer() const;
private:
	std::weak_ptr<const AbstractMaterial> m_material;
	std::unique_ptr<MeshBuffer> m_mesh_buffer;
};

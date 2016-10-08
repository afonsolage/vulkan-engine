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

	void set_material(std::shared_ptr<AbstractMaterial>& material);
	std::weak_ptr<AbstractMaterial> get_material() { return m_material; }
	const std::vector<char>& get_buffer() const;
	const std::vector<uint16_t>& get_indexes() const;

	template<typename T>
	uint32_t set_buffer(uint32_t location, const std::vector<T>& buffer);
	void set_indexes(std::vector<uint16_t>& buffer);

private:
	std::weak_ptr<AbstractMaterial> m_material;
	std::unique_ptr<MeshBuffer> m_mesh_buffer;
};

template<typename T>
inline uint32_t MeshComponent::set_buffer(uint32_t location, const std::vector<T>& buffer)
{
	return m_mesh_buffer->set(location, buffer);
}
#include "stdafx.h"
#include "MeshComponent.h"
#include "AbstractMaterial.h"

MeshComponent::MeshComponent()
{
	LOG_TRACE("Mesh component created.");
}

MeshComponent::~MeshComponent()
{
	LOG_TRACE("Mesh component destroyed.");
}

void MeshComponent::update()
{
	LOG_TRACE("Mesh component updated.");
}

void MeshComponent::on_attach()
{
	LOG_TRACE("Mesh component attached.");
}

void MeshComponent::on_detach()
{
	LOG_TRACE("Mesh component detached.");
}

void MeshComponent::set_material(std::shared_ptr<const AbstractMaterial>& material)
{
	m_material = material;

	m_mesh_buffer = std::make_unique<MeshBuffer>(material->create_mesh_buffer());
}

const std::vector<char>& MeshComponent::get_buffer() const
{
	if (!m_mesh_buffer->is_initialized())
	{
		LOG_FATAL("Current mesh component has no mesh buffer. Have you forgotten the material?");
		throw std::invalid_argument("Invalid mesh buffer.");
	}
	
	return m_mesh_buffer->get();
}

const std::vector<uint16_t>& MeshComponent::get_indexes() const
{
	if (!m_mesh_buffer->is_initialized())
	{
		LOG_FATAL("Current mesh component has no mesh buffer. Have you forgotten the material?");
		throw std::invalid_argument("Invalid mesh buffer.");
	}

	return m_mesh_buffer->get_indexes();
}

void MeshComponent::set_indexes(const std::vector<uint16_t>& buffer)
{
	m_mesh_buffer->set_indexes(buffer);
}


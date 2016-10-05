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

	m_mesh_buffer = material->create_mesh_buffer();
}

const std::vector<char>& MeshComponent::get_buffer() const
{
	if (!m_mesh_buffer.is_initialized())
	{
		LOG_FATAL("Current mesh component has no mesh buffer.");
		throw std::invalid_argument("Invalid mesh buffer.");
	}
	else
	{
		m_mesh_buffer.get();
	}
}


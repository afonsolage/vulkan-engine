#include "stdafx.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Entity.h"


CameraComponent::CameraComponent()
{
}


CameraComponent::~CameraComponent()
{
}

void CameraComponent::update()
{
}

void CameraComponent::setup(float width, float height, float field_of_view, float z_near, float z_far)
{
	m_projection = glm::perspective(field_of_view, width / height, z_near, z_far);
}

glm::mat4 CameraComponent::get_view()
{
	auto transform = m_transform.lock();

	if (!transform)
	{
		LOG_ERROR("Invalid transform on camera component!");
		return glm::mat4();
	}

	//Since this is a camera transform, it'll return the view matrix instead of component matrix;
	return transform->get_model();
}

void CameraComponent::on_attach()
{
	auto entity = m_entity.lock();

	if (!entity)
	{
		LOG_FATAL("Invalid entity received!");
		throw std::runtime_error("Entity is null.");
	}

	m_transform.swap(entity->get_component<TransformComponent>());
	
	auto transform = m_transform.lock();

	if (!transform)
	{
		LOG_FATAL("Invalid transform received!");
		throw std::runtime_error("Transofmr is null.");
	}
	else if (!transform->is_camera_transform())
	{
		LOG_FATAL("Transform isn't a camera transform!");
		throw std::invalid_argument("Transform must be a camera transform! Pass TRUE on TransformComponent constructor.");
	}
}

void CameraComponent::on_detach()
{
}

std::vector<const type_info*> CameraComponent::get_required_components()
{
	return std::vector<const type_info*>{ &typeid(TransformComponent) };
}

#include "stdafx.h"
#include "MeshComponent.h"


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


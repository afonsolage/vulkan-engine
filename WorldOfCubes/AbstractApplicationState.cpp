#include "stdafx.h"
#include "AbstractApplicationState.h"
#include "Application.h"

AbstractApplicationState::AbstractApplicationState(std::shared_ptr<Application> application)
	: m_application(application)
	, m_engine(application->get_engine())
	, m_terminated(false)
{

}

AbstractApplicationState::~AbstractApplicationState()
{

}

void AbstractApplicationState::initialize()
{
	if (m_terminated)
	{
		return;
	}

	on_initialized();
}

void AbstractApplicationState::update(float delta)
{
	if (m_terminated)
	{
		return;
	}

	on_updated(delta);
}

void AbstractApplicationState::terminate()
{
	if (m_terminated)
	{
		return;
	}

	m_terminated = true;
	on_terminated();

}

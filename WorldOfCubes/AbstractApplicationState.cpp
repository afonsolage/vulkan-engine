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
	on_initialized();
}

void AbstractApplicationState::update(float delta)
{
	on_updated(delta);
}

void AbstractApplicationState::terminate()
{
	on_terminated();
	m_terminated = true;
}

#include "stdafx.h"
#include "AbstractApplicationState.h"

AbstractApplicationState::AbstractApplicationState(std::shared_ptr<Application> application)
	: m_application(application)
{

}

AbstractApplicationState::~AbstractApplicationState()
{

}

void AbstractApplicationState::initialize()
{
}

void AbstractApplicationState::update(float delta)
{
}

void AbstractApplicationState::terminate()
{
	m_terminated = true;
}

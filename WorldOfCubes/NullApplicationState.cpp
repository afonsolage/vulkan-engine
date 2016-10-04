#include "stdafx.h"
#include "NullApplicationState.h"

NullApplicationState::NullApplicationState(std::shared_ptr<Application> application)
	: AbstractApplicationState(application)
{

}

void NullApplicationState::on_initialized()
{
}

void NullApplicationState::on_updated(float delta)
{
}

void NullApplicationState::on_terminated()
{
}

NullApplicationState::~NullApplicationState()
{
}

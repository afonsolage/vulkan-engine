#pragma once
#include "AbstractApplicationState.h"

class NullApplicationState :
	public AbstractApplicationState
{
public:
	NullApplicationState(std::shared_ptr<Application> application);

	virtual void on_initialized() override;
	virtual void on_updated(float delta) override;
	virtual void on_terminated() override;

	virtual ~NullApplicationState();
};


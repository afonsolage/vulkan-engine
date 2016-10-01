#pragma once

#include "AbstractApplicationState.h"

class TestApplicationState : public AbstractApplicationState
{
public:
	TestApplicationState(std::shared_ptr<Application> application);
	virtual ~TestApplicationState();

private:
	virtual void on_initialized() override;
	virtual void on_updated(float delta) override;
	virtual void on_terminated() override;

	int m_execution_order;
	bool m_update_first_run;
	int m_update_count;

	const int CONSTRUCTOR_ORDER = 0;
	const int ON_INITIALIZED_ORDER = 1;
	const int ON_TERMINATED_ORDER = 2;
	const int DESTRUCTOR_ORDER = 3;
};


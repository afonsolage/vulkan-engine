#pragma once
#include "AbstractComponent.h"

class TestComponent : public AbstractComponent
{
public:
	TestComponent();
	virtual ~TestComponent();

	void update() override;
	void on_attach() override;
	void on_detach() override;

	int m_execution_order;
	bool m_update_first_run;
	int m_update_count;

	const int CONSTRUCTOR_ORDER = 0;
	const int ON_ATTACH_ORDER = 1;
	const int ON_DETACH_ORDER = 2;
	const int DESTRUCTOR_ORDER = 3;
};


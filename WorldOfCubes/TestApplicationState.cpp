#include "stdafx.h"
#include "TestApplicationState.h"
#include <boost/test/unit_test.hpp>

TestApplicationState::TestApplicationState(std::shared_ptr<Application> application)
	: AbstractApplicationState(application)
	, m_execution_order(0)
	, m_update_first_run(false)
	, m_update_count(0)
{
	BOOST_CHECK(m_execution_order++ == CONSTRUCTOR_ORDER);
}

TestApplicationState::~TestApplicationState()
{
	BOOST_CHECK(m_execution_order == (DESTRUCTOR_ORDER + m_update_count));
}

void TestApplicationState::on_initialized()
{
	BOOST_CHECK(m_execution_order++ == ON_INITIALIZED_ORDER);
}

void TestApplicationState::on_updated(float delta)
{
	BOOST_CHECK(m_execution_order++ == (ON_INITIALIZED_ORDER + ++m_update_count));
}

void TestApplicationState::on_terminated()
{
	BOOST_CHECK(m_execution_order++ == (ON_TERMINATED_ORDER + m_update_count));
}
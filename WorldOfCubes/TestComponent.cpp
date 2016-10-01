#include "stdafx.h"
#include "TestComponent.h"
#include <boost/test/unit_test.hpp>

TestComponent::TestComponent()
	: m_execution_order(0)
	, m_update_first_run(false)
	, m_update_count(0)
{
	BOOST_CHECK(m_execution_order++ == CONSTRUCTOR_ORDER);
	BOOST_CHECK(!is_attached());
}


TestComponent::~TestComponent()
{
	BOOST_CHECK(m_execution_order == (DESTRUCTOR_ORDER + m_update_count));
	BOOST_CHECK(!is_attached());
}

void TestComponent::update()
{
	BOOST_CHECK(m_execution_order++ == (ON_ATTACH_ORDER + ++m_update_count));
}

void TestComponent::on_attach()
{
	BOOST_CHECK(is_attached());
	BOOST_CHECK(m_execution_order++ == ON_ATTACH_ORDER);
}

void TestComponent::on_detach()
{
	BOOST_CHECK(!is_attached());
	BOOST_CHECK(m_execution_order++ == (ON_DETACH_ORDER + m_update_count));
}

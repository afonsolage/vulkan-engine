#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>
#include "Fixtures.h"

BOOST_AUTO_TEST_SUITE(application_state_test_suite)

BOOST_FIXTURE_TEST_CASE(application_test, BasicAppFixture)
{
	m_application->change_state<TestApplicationState>();
	m_application->change_state<TestApplicationState>();
	BOOST_CHECK(m_application->is_state<TestApplicationState>());
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
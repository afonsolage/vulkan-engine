#include "stdafx.h"
#define BOOST_TEST_MODULE world of cube test module
#include <boost/test/unit_test.hpp>
#include "Fixtures.h"

std::shared_ptr<Application> BasicAppFixture::m_application;
std::weak_ptr<GameEngine> BasicAppFixture::m_engine;
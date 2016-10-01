#pragma once
#include "stdafx.h"
#include "Application.h"
#include "TestApplicationState.h"
#include "GameEngine.h"

struct BasicAppFixture
{
	BasicAppFixture()
	{
		m_test_thread_id = std::this_thread::get_id();
		m_application = std::make_shared<Application>();
		m_application->init();
	}

	virtual ~BasicAppFixture()
	{
		auto engine = m_application->get_engine().lock();

		if (engine)
		{
			engine->finalized();
		}
	}

	bool is_test_thread() { return m_test_thread_id == std::this_thread::get_id(); }

	std::shared_ptr<Application> m_application;
	std::thread::id m_test_thread_id;
};

struct TestAppFixture : BasicAppFixture
{
	TestAppFixture() : BasicAppFixture()
	{
		m_application->change_state<TestApplicationState>();
	}

	virtual ~TestAppFixture()
	{

	}
};
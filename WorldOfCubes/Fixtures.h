#pragma once
#include "stdafx.h"
#include "Application.h"
#include "TestApplicationState.h"
#include "GameEngine.h"

struct BasicAppFixture
{
	BasicAppFixture()
	{
		m_application = std::make_shared<Application>();
		m_application->init();
		m_engine = m_application->get_engine();
	}

	virtual ~BasicAppFixture()
	{
		auto engine = m_application->get_engine().lock();

		if (engine)
		{
			engine->finalized();
		}
	}

	std::shared_ptr<Application> m_application;
	std::weak_ptr<GameEngine> m_engine;
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
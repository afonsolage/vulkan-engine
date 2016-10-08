#pragma once
#include "stdafx.h"
#include "Application.h"
#include "TestApplicationState.h"
#include "GameEngine.h"
#include "WorldApplicationState.h"

struct BasicAppFixture
{
	BasicAppFixture()
	{
		if (!m_application)
		{
			m_application = std::make_shared<Application>();
			m_application->init();

			m_application->change_state<WorldApplicationState>();

		}
		if (m_engine.expired())
		{
			m_engine = m_application->get_engine();
		}
	}

	static std::shared_ptr<Application> m_application;
	static std::weak_ptr<GameEngine> m_engine;
};
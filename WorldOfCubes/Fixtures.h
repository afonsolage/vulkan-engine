#pragma once
#include "stdafx.h"
#include "Application.h"
#include "TestApplicationState.h"
#include "GameEngine.h"

struct BasicAppFixture
{
	BasicAppFixture()
	{
		if (!m_application)
		{
			m_application = std::make_shared<Application>();
			m_application->init();
			m_engine = m_application->get_engine();
		}
	}

	static std::shared_ptr<Application> m_application;
	static std::weak_ptr<GameEngine> m_engine;
};
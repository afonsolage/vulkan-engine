#pragma once

//Forwarding declarations
class GameEngine;

class WorldOfCubes
{
public:
	WorldOfCubes();

	virtual ~WorldOfCubes();

	void init();
	void run();

private:
	std::shared_ptr<GameEngine> m_engine;

	const std::string m_app_name = "World of Cubes";
	const uint32_t m_app_version = MAKE_VERSION(0, 1, 0);
};

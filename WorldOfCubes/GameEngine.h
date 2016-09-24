#pragma once

#include <glm/glm.hpp>

//Forwarding declarations
class WindowSystem;
class GraphicsSystem;
class FileSystem;

class GameEngine : public std::enable_shared_from_this<GameEngine>
{

public:
	GameEngine(std::string app_name, uint32_t app_version);

	virtual ~GameEngine();

	inline bool is_debug_mode() const;
	std::string get_app_name() const { return m_app_name; }
	uint32_t get_app_version() const { return m_app_version; }
	std::string get_engine_name() const { return m_engine_name; }
	uint32_t get_engine_version() const { return m_engine_version; }

	void init();
	void tick();
	bool is_running();

	std::weak_ptr<WindowSystem> get_window_system() { return m_window_system; };
	std::weak_ptr<FileSystem> get_file_system() { return m_file_system; }

	uint32_t get_window_width() const { return m_window_size.x; };
	uint32_t get_window_height() const { return m_window_size.y; };

private:

	const std::string m_app_name;
	const uint32_t m_app_version;
	const std::string m_engine_name = "WOC Engine";
	const uint32_t m_engine_version = MAKE_VERSION(0, 1, 0);

	glm::uvec2 m_window_size;

	std::shared_ptr<GraphicsSystem> m_graphic_system;
	std::shared_ptr<WindowSystem> m_window_system;
	std::shared_ptr<FileSystem> m_file_system;
};

inline
bool GameEngine::is_debug_mode() const
{
#ifndef NDEBUG
	return true;
#else
	return false;
#endif
}


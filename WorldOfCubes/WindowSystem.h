#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//Forwarding declaration;
class Context;

class WindowSystem : public std::enable_shared_from_this<WindowSystem>
{

public:
	WindowSystem();
	virtual ~WindowSystem();

	std::vector<const char*> get_required_extensions();

	void init(const char* window_title, int width, int height);
	void tick();
	bool close_requested();
	void create_surface(VkInstance instance, VkSurfaceKHR* surface);

private:

	using GLFWPwindow = std::unique_ptr<GLFWwindow, std::decay_t<decltype(glfwDestroyWindow)>>;
	GLFWPwindow m_window;
};


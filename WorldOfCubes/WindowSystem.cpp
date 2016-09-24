#include "stdafx.h"
#include "WindowSystem.h"


WindowSystem::WindowSystem()
	: m_window(nullptr, glfwDestroyWindow)
{

}


WindowSystem::~WindowSystem()
{
	LOG_DEBUG("Window system destroyed.");
}

std::vector<const char*> WindowSystem::get_required_extensions()
{
	uint32_t extensions_count = 0;
	auto extensions = glfwGetRequiredInstanceExtensions(&extensions_count);

	std::vector<const char*> result;
	for (uint32_t i = 0; i < extensions_count; i++)
	{
		result.emplace_back(extensions[i]);
	}

	return result;
}

void WindowSystem::init(const char* window_title, int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	
	m_window.reset(glfwCreateWindow(width, height, window_title, nullptr, nullptr));
}

void WindowSystem::tick()
{
	glfwPollEvents();
}

bool WindowSystem::close_requested()
{
	return glfwWindowShouldClose(m_window.get()) == GLFW_TRUE;
}

void WindowSystem::create_surface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, m_window.get(), nullptr, surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create surface!");
	}
}
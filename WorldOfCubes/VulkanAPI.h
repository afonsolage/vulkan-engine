#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

struct QueueFamilyIndices
{
	uint32_t graphics = UINT32_MAX;
	uint32_t presentation = UINT32_MAX;

	operator bool() const
	{
		return graphics != UINT32_MAX && presentation != UINT32_MAX;
	}

	std::vector<uint32_t> get_indices()
	{
		return{ graphics, presentation };
	}

	std::set<uint32_t> get_indices_unique()
	{
		return{ graphics, presentation };
	}
};

struct SwapchainSupportDetails
{
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> present_modes;

	operator bool() const
	{
		return !formats.empty() && !present_modes.empty();
	}
};

class VKUtils
{
public:
	static QueueFamilyIndices get_family_indices(const vk::PhysicalDevice& physical_device, const vk::SurfaceKHR& surface);
	static SwapchainSupportDetails get_swapchain_support(const vk::PhysicalDevice& physical_device, const vk::SurfaceKHR& surface);
	static std::vector<char> load_shader_file(const char* file_name);
};
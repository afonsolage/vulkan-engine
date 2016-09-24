#include "stdafx.h"
#include "VulkanAPI.h"

QueueFamilyIndices VKUtils::get_family_indices(const vk::PhysicalDevice& physical_device, const vk::SurfaceKHR& surface)
{
	auto queue_families = physical_device.getQueueFamilyProperties();

	QueueFamilyIndices indices;

	uint32_t i = 0;
	for (const auto& queue_family : queue_families)
	{
		if (queue_family.queueCount > 0)
		{
			if (queue_family.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				indices.graphics = i;

				//In order to be a valid presentation queue, this one also must have graphics capabilities.
				if (physical_device.getSurfaceSupportKHR(i, surface))
				{
					//it's better to have presentation and graphics on same queue family.
					if (indices.presentation == std::numeric_limits<uint32_t>::max()
						|| (indices.presentation != indices.graphics && i == indices.graphics))
					{
						indices.presentation = i;
					}
				}
			}

			if (queue_family.queueFlags & vk::QueueFlagBits::eTransfer)
			{
				//it's better to have a queue family that only does transfer, because it have some improvments.
				if (indices.transfer == std::numeric_limits<uint32_t>::max()
					|| indices.transfer == indices.graphics
					|| queue_family.queueFlags == vk::QueueFlagBits::eTransfer)
				{
					indices.transfer = i;
				}
			}
		}

		i++;
	}

	return indices;
}

SwapchainSupportDetails VKUtils::get_swapchain_support(const vk::PhysicalDevice & physical_device, const vk::SurfaceKHR& surface)
{
	SwapchainSupportDetails details;

	details.capabilities = physical_device.getSurfaceCapabilitiesKHR(surface);
	details.formats = physical_device.getSurfaceFormatsKHR(surface);
	details.present_modes = physical_device.getSurfacePresentModesKHR(surface);

	return details;
}

std::vector<char> VKUtils::load_shader_file(const char * file_name)
{
	std::ifstream file(file_name, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("Failed to open file!");

	size_t file_size = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(file_size);

	file.seekg(0);
	file.read(buffer.data(), file_size);
	file.close();

	return buffer;
}

#pragma once

#include "VulkanAPI.h"

//Forwarding declarations
class GameEngine;

class Context
{
public:
	Context(std::shared_ptr<GameEngine>& engine);

	virtual ~Context();

	void init();


	void transition_image_layout(vk::Image image, vk::ImageLayout src_layout, vk::ImageLayout dst_layout);
	void create_image(vk::Extent3D extent, vk::ImageTiling tiling, vk::Format format, vk::ImageUsageFlags usage_flags, 
		vk::MemoryPropertyFlags property_flags, vk::Image& image, vk::DeviceMemory& buffer) const;
	vk::ImageView create_image_view(vk::Image image, vk::Format format, vk::ImageAspectFlags aspect_flags) const;
	vk::Format find_supported_format(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags flags) const;
	uint32_t find_memory_index(uint32_t type, vk::MemoryPropertyFlags prop_flags) const;
	vk::MemoryRequirements get_memory_requirements(vk::Image image) const;


	vk::Instance m_instance;
	vk::SurfaceKHR m_surface;
	vk::PhysicalDevice m_physical_device;
	vk::Device m_device;
	vk::Semaphore m_image_available_semaphore;
	vk::Semaphore m_render_finished_semaphore;
	vk::Queue m_present_queue;
	vk::Queue m_graphics_queue;

	vk::CommandPool m_graphics_command_pool;
	vk::CommandPool m_present_command_pool;
	vk::CommandPool m_transfer_queue_command;

	QueueFamilyIndices m_queue_family_indices;

	static const vk::Format m_presentation_format = vk::Format::eR8G8B8A8Unorm;
private:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData);

	void create_instance();
	void setup_debug_callback();
	void create_surface();
	void find_physical_device();
	void create_logical_device();
	void create_semaphores();
	void create_command_pool();

	void destroy_command_pool();

	int calc_physical_device_rank(const vk::PhysicalDevice& physical_device) const;

	const std::vector<const char*> m_validation_layers = { "VK_LAYER_LUNARG_standard_validation" };
	const std::vector<const char*> m_required_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	bool m_debug_enabled;

	std::weak_ptr<GameEngine> m_engine;

	VkDebugReportCallbackEXT m_callback;

};


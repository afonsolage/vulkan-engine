#include "stdafx.h"
#include "Context.h"
#include "GameEngine.h"
#include "WindowSystem.h"

PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallbackEXT = VK_NULL_HANDLE;
PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallbackEXT = VK_NULL_HANDLE;

Context::Context(std::shared_ptr<GameEngine>& engine)
	: m_engine(engine)
	, m_debug_enabled(false)
{

}

Context::~Context()
{
	destroy_command_pool();

	m_device.destroySemaphore(m_image_available_semaphore);
	m_device.destroySemaphore(m_render_finished_semaphore);

	m_device.destroy();

	m_instance.destroySurfaceKHR(m_surface);

	if (DestroyDebugReportCallbackEXT != VK_NULL_HANDLE)
		DestroyDebugReportCallbackEXT((VkInstance)m_instance, m_callback, nullptr);

	m_instance.destroy();

	LOG_DEBUG("Context destroyed.");
}

void Context::init()
{
	create_instance();
	setup_debug_callback();
	create_surface();
	find_physical_device();
	create_logical_device();
	create_semaphores();
	create_command_pool();
}

void Context::create_instance()
{
	vk::ApplicationInfo app_info;

	auto engine = m_engine.lock();
	assert(engine != nullptr);

	m_debug_enabled = engine->is_debug_mode();

	//Application info
	app_info.apiVersion = VK_API_VERSION_1_0;
	app_info.pApplicationName = engine->get_app_name().c_str();
	app_info.applicationVersion = engine->get_app_version();
	app_info.pEngineName = engine->get_engine_name().c_str();
	app_info.engineVersion = engine->get_engine_version();

	auto window_system = engine->get_window_system().lock();
	assert(window_system != nullptr);

	//Extensions 
	auto required_extensions = window_system->get_required_extensions();

	if (m_debug_enabled)
	{
		required_extensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	vk::InstanceCreateInfo create_info;

	create_info.pApplicationInfo = &app_info;
	create_info.enabledExtensionCount = required_extensions.size();
	create_info.ppEnabledExtensionNames = required_extensions.data();

	if (m_debug_enabled)
	{
		create_info.enabledLayerCount = m_validation_layers.size();
		create_info.ppEnabledLayerNames = m_validation_layers.data();
	}

	m_instance = vk::createInstance(create_info);
}

void Context::setup_debug_callback()
{
	auto engine = m_engine.lock();
	assert(engine != nullptr);

	if (!engine->is_debug_mode())
		return;

	CreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)m_instance.getProcAddr("vkCreateDebugReportCallbackEXT");
	DestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)m_instance.getProcAddr("vkDestroyDebugReportCallbackEXT");

	vk::DebugReportCallbackCreateInfoEXT createInfo;
	createInfo.pfnCallback = Context::debugCallback;
	createInfo.flags = vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::ePerformanceWarning;

	auto cbInfo = (const VkDebugReportCallbackCreateInfoEXT&)createInfo;

	if (CreateDebugReportCallbackEXT((VkInstance)m_instance, &cbInfo, nullptr, &m_callback) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create debug report callback!");
	}
}

void Context::create_surface()
{
	auto engine = m_engine.lock();
	assert(engine != nullptr);

	auto window_system = engine->get_window_system().lock();
	assert(window_system != nullptr);

	window_system->create_surface((VkInstance)m_instance, &(VkSurfaceKHR&)m_surface);
}

void Context::find_physical_device()
{
	auto available_devices = m_instance.enumeratePhysicalDevices();

	std::vector<std::pair<int, vk::PhysicalDevice>> devices_ranked;

	for (const auto& physical_device : available_devices)
	{
		devices_ranked.emplace_back(calc_physical_device_rank(physical_device), physical_device);
	}

	std::sort(begin(devices_ranked), end(devices_ranked), [](const auto& dev1, const auto& dev2)
	{
		return dev1.first < dev2.first;
	});

	auto ranked = devices_ranked.back();

	if (ranked.first < 0)
	{
		throw std::runtime_error("Failed to find suitable physical device!");
	}

	LOG_TRACE("A physical device with rank %d is begin used.", ranked.first);

	m_physical_device = ranked.second;
	m_queue_family_indices = VKUtils::get_family_indices(m_physical_device, m_surface);
}

void Context::create_logical_device()
{
	auto unique_indices = m_queue_family_indices.get_indices_unique();
	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;

	auto queue_priority = 1.0f;

	for (const auto& index : unique_indices)
	{
		vk::DeviceQueueCreateInfo create_info;
		create_info.queueFamilyIndex = index;
		create_info.queueCount = 1;
		create_info.pQueuePriorities = &queue_priority;

		queue_create_infos.emplace_back(create_info);
	}

	vk::DeviceCreateInfo create_info;
	create_info.queueCreateInfoCount = queue_create_infos.size();
	create_info.pQueueCreateInfos = queue_create_infos.data();
	create_info.pEnabledFeatures = &vk::PhysicalDeviceFeatures();
	create_info.enabledExtensionCount = m_required_extensions.size();
	create_info.ppEnabledExtensionNames = m_required_extensions.data();

	if (m_debug_enabled)
	{
		create_info.enabledLayerCount = m_validation_layers.size();
		create_info.ppEnabledLayerNames = m_validation_layers.data();
	}

	m_device = m_physical_device.createDevice(create_info);

	m_graphics_queue = m_device.getQueue(m_queue_family_indices.graphics, 0);
	m_present_queue = m_device.getQueue(m_queue_family_indices.presentation, 0);
}

void Context::create_semaphores()
{
	m_image_available_semaphore = m_device.createSemaphore(vk::SemaphoreCreateInfo());
	m_render_finished_semaphore = m_device.createSemaphore(vk::SemaphoreCreateInfo());
}

void Context::create_command_pool()
{
	vk::CommandPoolCreateInfo create_info;
	create_info.queueFamilyIndex = m_queue_family_indices.graphics;

	m_graphics_command_pool = m_device.createCommandPool(create_info);

	if (m_queue_family_indices.presentation == m_queue_family_indices.graphics)
	{
		m_transfer_queue_command = m_graphics_command_pool;
	}
	else
	{
		create_info.queueFamilyIndex = m_queue_family_indices.presentation;
		m_present_command_pool = m_device.createCommandPool(create_info);
	}

	if (m_queue_family_indices.transfer == m_queue_family_indices.graphics)
	{
		m_transfer_queue_command = m_graphics_command_pool;
	}
	else if (m_queue_family_indices.transfer == m_queue_family_indices.presentation)
	{
		m_transfer_queue_command = m_present_command_pool;
	}
	else
	{
		create_info.queueFamilyIndex = m_queue_family_indices.transfer;
		m_transfer_queue_command = m_device.createCommandPool(create_info);
	}
}

void Context::destroy_command_pool()
{
	m_device.destroyCommandPool(m_graphics_command_pool);

	if (m_queue_family_indices.presentation != m_queue_family_indices.graphics)
	{
		m_device.destroyCommandPool(m_present_command_pool);
	}

	if (m_queue_family_indices.transfer != m_queue_family_indices.graphics
		&& m_queue_family_indices.transfer != m_queue_family_indices.presentation)
	{
		m_device.destroyCommandPool(m_transfer_queue_command);
	}
}

int Context::calc_physical_device_rank(const vk::PhysicalDevice& physical_device) const
{
	//Check queue family indices.
	auto indices = VKUtils::get_family_indices(physical_device, m_surface);
	if (!indices)
		return -1;

	//Check required extensions.
	auto available_extensions = physical_device.enumerateDeviceExtensionProperties();
	for (const auto& required_extension : m_required_extensions)
	{
		auto result = std::any_of(begin(available_extensions), end(available_extensions), [&](const auto& prop)
		{
			return strcmp(required_extension, prop.extensionName) == 0;
		});

		//If the required extensions isn't available, than this physical_device isn't supported.
		if (!result)
			return -1;
	}

	auto surface_support = VKUtils::get_swapchain_support(physical_device, m_surface);
	if (!surface_support)
		return -1;


	int rank = 0;

	//We should rank better physical devices that have the graphics and presentation on same queue family index.
	rank = (indices.graphics == indices.presentation) ? 10 : 0;

	//We should rank better physical devices that have the graphics and transfer operations on different families.
	rank = (indices.transfer != indices.graphics) ? 10 : 0;

	bool mailbox_support = std::any_of(begin(surface_support.present_modes), end(surface_support.present_modes), [](const auto& present_mode)
	{
		return present_mode == vk::PresentModeKHR::eMailbox;
	});

	//Devices which got mailbox support, also rank better.
	rank += (mailbox_support) ? 10 : 0;

	bool format_support = std::any_of(begin(surface_support.formats), end(surface_support.formats), [](const auto& format)
	{
		return format == Context::m_presentation_format;
	});

	//Devices which got our desired present support rank better.
	rank += (format_support) ? 10 : 0;


	//Devices with no extent limit should rank better. Note if width is undefined (max uint32_t), height is also.
	rank += (surface_support.capabilities.currentExtent.width == std::numeric_limits<uint32_t>::max()) ? 10 : 0;

	//TODO: Add another metrics for ranking better the available physical devices.

	return rank;
}

vk::ImageView Context::create_image_view(vk::Image image, vk::Format format, vk::ImageAspectFlags aspect_flags)
{
	vk::ImageViewCreateInfo create_info;
	create_info.image = image;
	create_info.format = format;
	create_info.viewType = vk::ImageViewType::e2D;
	create_info.subresourceRange.aspectMask = aspect_flags;
	create_info.subresourceRange.baseArrayLayer = 0;
	create_info.subresourceRange.layerCount = 1;
	create_info.subresourceRange.baseMipLevel = 0;
	create_info.subresourceRange.levelCount = 1;

	return m_device.createImageView(create_info);
}

vk::Format Context::find_supported_format(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags flags) const
{
	vk::FormatProperties props;

	for (const auto& format : candidates)
	{
		props = m_physical_device.getFormatProperties(format);

		if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & flags) == flags)
		{
			return format;
		}
		else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & flags) == flags)
		{
			return format;
		}
	}

	throw std::runtime_error("Failed to find supported format!");
}

VKAPI_ATTR VkBool32 VKAPI_CALL Context::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData)
{
	if (flags && VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		LOG_ERROR("[%d] %s", code, msg);
	}
	else if (flags && VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		LOG_WARN("[%d] %s", code, msg);
	}
	else if (flags && VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		LOG_WARN("[PERFORMANCE][%d] %s", code, msg);
	}


	return false;
}
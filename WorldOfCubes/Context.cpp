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

	m_device.destroyFence(m_single_usage_fence);
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
	create_fence();
	create_command_pool();
}

vk::CommandBuffer Context::begin_single_use_command_buffer()
{
	vk::CommandBufferAllocateInfo alloc_info;
	alloc_info.level = vk::CommandBufferLevel::ePrimary;
	alloc_info.commandPool = m_graphics_command_pool;
	alloc_info.commandBufferCount = 1;

	auto command_buffer = m_device.allocateCommandBuffers(alloc_info).front();

	vk::CommandBufferBeginInfo begin_info;
	begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

	command_buffer.begin(begin_info);

	return command_buffer;
}

void Context::end_single_use_command_buffer(vk::CommandBuffer & command_buffer)
{
	command_buffer.end();

	vk::SubmitInfo submit_info;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &command_buffer;

	m_graphics_queue.submit(submit_info, m_single_usage_fence);

	m_device.waitForFences(m_single_usage_fence, VK_TRUE, std::numeric_limits<uint64_t>::max());
	m_device.resetFences(m_single_usage_fence);
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

void Context::create_fence()
{
	m_single_usage_fence = m_device.createFence(vk::FenceCreateInfo());
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

void Context::transition_image_layout(vk::Image image, vk::ImageLayout src_layout, vk::ImageLayout dst_layout)
{
	auto command_buffer = begin_single_use_command_buffer();

	vk::ImageMemoryBarrier barrier;
	barrier.oldLayout = src_layout;
	barrier.newLayout = dst_layout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = (dst_layout == vk::ImageLayout::eDepthStencilAttachmentOptimal) ? vk::ImageAspectFlagBits::eDepth : vk::ImageAspectFlagBits::eColor;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	if (src_layout == vk::ImageLayout::ePreinitialized && dst_layout == vk::ImageLayout::eTransferSrcOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eHostRead;
	}
	else if (src_layout == vk::ImageLayout::ePreinitialized && dst_layout == vk::ImageLayout::eTransferDstOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eHostWrite;
	}
	else if (src_layout == vk::ImageLayout::eTransferDstOptimal && dst_layout == vk::ImageLayout::eShaderReadOnlyOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
	}
	else if (src_layout == vk::ImageLayout::eUndefined && dst_layout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
	{
		barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
	}

	command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTopOfPipe, vk::DependencyFlags(), nullptr, nullptr, barrier);
	end_single_use_command_buffer(command_buffer);
}

void Context::create_image(vk::Extent3D extent, vk::ImageTiling tiling, vk::Format format, vk::ImageUsageFlags usage_flags, vk::MemoryPropertyFlags property_flags, vk::Image & image, vk::DeviceMemory & buffer) const
{
	vk::ImageCreateInfo create_info;

	create_info.imageType = vk::ImageType::e2D;
	create_info.extent = extent;
	create_info.mipLevels = 1;
	create_info.arrayLayers = 1;
	create_info.format = format;
	create_info.tiling = tiling;
	create_info.initialLayout = vk::ImageLayout::ePreinitialized;
	create_info.usage = usage_flags;
	create_info.sharingMode = vk::SharingMode::eExclusive;
	create_info.samples = vk::SampleCountFlagBits::e1;

	image = m_device.createImage(create_info);

	auto memory_requirements = get_memory_requirements(image);

	vk::MemoryAllocateInfo alloc_info;
	alloc_info.allocationSize = memory_requirements.size;
	alloc_info.memoryTypeIndex = find_memory_index(memory_requirements.memoryTypeBits, property_flags);

	buffer = m_device.allocateMemory(alloc_info);

	m_device.bindImageMemory(image, buffer, vk::DeviceSize{ 0 });
}

vk::ImageView Context::create_image_view(vk::Image image, vk::Format format, vk::ImageAspectFlags aspect_flags) const
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

uint32_t Context::find_memory_index(uint32_t type, vk::MemoryPropertyFlags prop_flags) const
{
	auto mem_properies = m_physical_device.getMemoryProperties();

	for (uint32_t i = 0; i < mem_properies.memoryTypeCount; i++)
	{
		if (type & (1 << i) && (mem_properies.memoryTypes[i].propertyFlags & prop_flags) == prop_flags)
		{
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type.");
}

vk::MemoryRequirements Context::get_memory_requirements(vk::Image image) const
{
	return m_device.getImageMemoryRequirements(image);
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
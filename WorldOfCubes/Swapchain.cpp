#include "stdafx.h"
#include "Swapchain.h"
#include "Context.h"
#include "GraphicsSystem.h"

Swapchain::Swapchain(std::shared_ptr<GraphicsSystem>& graphics_system)
	: m_graphics_system(graphics_system)
{
	m_context = graphics_system->get_context();
}


Swapchain::~Swapchain()
{
	auto context = m_context.lock();

	if (context != nullptr)
	{
		clear_images(context->m_device);

		context->m_device.destroySwapchainKHR(m_swapchain);
	}

	LOG_DEBUG("Swapchain destroyed.");
}

void Swapchain::init()
{
	create_swapchain();
}

bool Swapchain::swap_images()
{
	GET_CONTEXT;

	uint32_t next_image;

	auto result = context->m_device.acquireNextImageKHR(m_swapchain,
		std::numeric_limits<uint64_t>::max(),
		context->m_image_available_semaphore,
		vk::Fence(),
		&next_image);

	//TODO: Add multithread sync?
	m_current_image_index = next_image;

	return is_swapchain_valid(result);
}

bool Swapchain::present()
{
	GET_CONTEXT;

	//TODO: Add multithread sync?
	auto current_image = m_current_image_index;

	vk::PresentInfoKHR present_info;

	context->m_image_available_semaphore;

	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = &context->m_image_available_semaphore;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = &m_swapchain;
	present_info.pImageIndices = &current_image;

	auto result = context->m_present_queue.presentKHR(present_info);

	return is_swapchain_valid(result);
}

vk::AttachmentDescription Swapchain::get_attachment_description() const noexcept
{
	vk::AttachmentDescription description;

	description.format = m_format;
	description.samples = vk::SampleCountFlagBits::e1; //TODO: Add settings for sampling
	description.loadOp = vk::AttachmentLoadOp::eClear;
	description.storeOp = vk::AttachmentStoreOp::eStore;
	description.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	description.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	description.initialLayout = vk::ImageLayout::eUndefined;
	description.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	return description;
}

void Swapchain::create_swapchain()
{
	GET_CONTEXT;

	SwapchainSupportDetails details = VKUtils::get_swapchain_support(context->m_physical_device, context->m_surface);

	auto graphic_system = m_graphics_system.lock();
	assert(graphic_system != nullptr);

	auto surface_format = get_best_matching_surface_format(details.formats);
	auto present_mode = get_best_matching_present_mode(details.present_modes);
	auto extent = get_best_matching_extent(details.capabilities, { graphic_system->get_window_width(), graphic_system->get_window_height() });

	auto image_count = details.capabilities.minImageCount + 1;

	if (details.capabilities.maxImageCount > 0 && image_count > details.capabilities.maxImageCount)
		image_count = details.capabilities.maxImageCount;

	QueueFamilyIndices indices = context->m_queue_family_indices;
	auto unique_family_indices = indices.get_indices_unique();

	std::vector<uint32_t> family_indices;
	std::copy(begin(unique_family_indices), end(unique_family_indices), std::back_inserter(family_indices));

	auto present = context->m_surface;

	vk::SwapchainKHR old_swap_chain = m_swapchain;

	vk::SwapchainCreateInfoKHR create_info;
	create_info.surface = present;
	create_info.minImageCount = image_count;
	create_info.imageFormat = surface_format.format;
	create_info.imageColorSpace = surface_format.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1;
	create_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
	create_info.imageSharingMode = vk::SharingMode::eExclusive;
	create_info.queueFamilyIndexCount = family_indices.size();
	create_info.pQueueFamilyIndices = family_indices.data();
	create_info.preTransform = details.capabilities.currentTransform;
	create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;
	create_info.oldSwapchain = m_swapchain;

	m_swapchain = context->m_device.createSwapchainKHR(create_info);
	m_format = surface_format.format;
	m_extent = extent;

	if (old_swap_chain)
	{
		clear_images(context->m_device);
	}

	auto images = context->m_device.getSwapchainImagesKHR(m_swapchain);
	image_count = images.size();

	m_images.resize(image_count);
	for (uint32_t i = 0; i < image_count; i++)
	{
		m_images[i].image = images[i];
		m_images[i].image_view = context->create_image_view(images[i], m_format, vk::ImageAspectFlagBits::eColor);
	}
}

void Swapchain::clear_images(const vk::Device& device)
{
	for_each(begin(m_images), end(m_images), [&](auto& image)
	{
		image.destroy(device);
	});
}

bool Swapchain::is_swapchain_valid(const vk::Result & result)
{
	if (result == vk::Result::eErrorOutOfDateKHR || result != vk::Result::eSuboptimalKHR)
	{
		return false;
	}
	else if (result != vk::Result::eSuccess)
	{
		throw std::runtime_error("Invalid swapchain result!");
	}

	return true;
}

void Swapchain::recreate()
{
	throw std::runtime_error("Not implemented yet!");
}

vk::SurfaceFormatKHR Swapchain::get_best_matching_surface_format(const std::vector<vk::SurfaceFormatKHR>& available_formats)
{
	//A single available format of type undefined means we have no restriction about surface format, then we can choose the best one.
	if (available_formats.size() == 1 && available_formats[0].format == vk::Format::eUndefined)
	{
		return{ Context::m_presentation_format, vk::ColorSpaceKHR::eSrgbNonlinear };
	}

	//Let's check if the best format for us is available.
	for (const auto& available_format : available_formats)
	{
		if (available_format.format == Context::m_presentation_format && available_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		{
			return available_format;
		}
	}

	//If isn't, get the first available one.
	return available_formats.front();
}

vk::PresentModeKHR Swapchain::get_best_matching_present_mode(const std::vector<vk::PresentModeKHR>& available_present_modes)
{
	//Let's check if this graphic card support's mailbox, this is the best one for us.

	for (const auto& available_present_mode : available_present_modes)
	{
		if (available_present_mode == vk::PresentModeKHR::eMailbox)
			return available_present_mode;
	}

	//If it doesn't support, every graphic card support FIFO, so let's use this one.
	return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Swapchain::get_best_matching_extent(const vk::SurfaceCapabilitiesKHR& capabilities, vk::Extent2D desired_extent)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		desired_extent.width = clamp(capabilities.minImageExtent.width, capabilities.maxImageExtent.width, desired_extent.width);
		desired_extent.height = clamp(capabilities.minImageExtent.height, capabilities.maxImageExtent.height, desired_extent.height);
	}

	return desired_extent;
}

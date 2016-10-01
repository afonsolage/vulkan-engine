#pragma once

#include "VulkanAPI.h"

//Forwarding declaration
class GraphicsSystem;
class Context;

struct SwapchainImage
{
	vk::Image image;
	vk::ImageView image_view;
	vk::Framebuffer framebuffer;

	void destroy(const vk::Device& device)
	{
		device.destroyFramebuffer(framebuffer);
		device.destroyImageView(image_view);
	}
};

class Swapchain
{
public:
	Swapchain(std::shared_ptr<GraphicsSystem>& graphic_system);

	virtual ~Swapchain();

	void init();
	bool swap_images();
	bool present();

	void create_framebuffer(vk::RenderPass& render_pass, vk::ImageView depth_image_view);

	vk::AttachmentDescription get_attachment_description() const noexcept;

	vk::Format m_format;
	vk::Extent2D m_extent;


private:

	void create_swapchain();
	void clear_images(const vk::Device& device);
	bool is_swapchain_valid(const vk::Result& result);
	void recreate();

	vk::SurfaceFormatKHR get_best_matching_surface_format(const std::vector<vk::SurfaceFormatKHR>& available_formats);
	vk::PresentModeKHR get_best_matching_present_mode(const std::vector<vk::PresentModeKHR>& available_present_modes);
	vk::Extent2D get_best_matching_extent(const vk::SurfaceCapabilitiesKHR& capabilities, vk::Extent2D desired_extent);

	std::weak_ptr<Context> m_context;
	std::weak_ptr<GraphicsSystem> m_graphics_system;

	vk::SwapchainKHR m_swapchain;

	std::vector<SwapchainImage> m_images;
	uint32_t m_current_image_index;
};


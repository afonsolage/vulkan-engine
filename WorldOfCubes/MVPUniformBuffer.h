#pragma once

#include "VulkanAPI.h"
#include "MathUtils.h"

class Context;

class MVPUniformBuffer
{
public:
	MVPUniformBuffer(std::weak_ptr<Context>& context);
	virtual ~MVPUniformBuffer();

	void set_model(const glm::mat4& model) { m_mvp.model = model; m_dirtiness.m_mvp.m = 1; };
	void set_view(const glm::mat4& view) { m_mvp.view = view;  m_dirtiness.m_mvp.v = 1; };
	void set_projection(const glm::mat4& projection) { m_mvp.projection = projection;  m_dirtiness.m_mvp.m = 1; };

	void sync_device_memory();

protected:
	typedef union
	{
		struct
		{
			unsigned char m : 1;
			unsigned char v : 1;
			unsigned char p : 1;
		} m_mvp;
		bool m_dirty;
	} Dirtiness;

	struct MVP
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
	};

	vk::Buffer m_buffer;
	vk::Buffer m_staging_buffer;

	vk::DeviceMemory m_buffer_memory;
	vk::DeviceMemory m_staging_buffer_memory;

	vk::DeviceSize m_offset;

	const vk::DeviceSize m_size_m;
	const vk::DeviceSize m_size_mv;
	const vk::DeviceSize m_size_mvp;

	std::weak_ptr<Context> m_context;

	MVP m_mvp;

	Dirtiness m_dirtiness;
};

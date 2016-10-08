#include "stdafx.h"
#include "MVPUniformBuffer.h"
#include "Context.h"

MVPUniformBuffer::MVPUniformBuffer(std::weak_ptr<Context>& context)
	: m_size_m(sizeof(m_mvp.model))
	, m_size_mv(m_size_m + sizeof(m_mvp.view))
	, m_size_mvp(m_size_mv + sizeof(m_mvp.projection))
	, m_context(context)
{
	m_dirtiness.m_mvp = { 1, 1, 1 };
}


MVPUniformBuffer::~MVPUniformBuffer()
{

}

void MVPUniformBuffer::sync_device_memory()
{
	if (!m_dirtiness.m_dirty)
		return;

	vk::DeviceSize copy_size;

	if (m_dirtiness.m_mvp.p == 1)
	{
		copy_size = m_size_mvp;
	}
	else if (m_dirtiness.m_mvp.v == 1)
	{
		copy_size = m_size_mv;
	}
	else if (m_dirtiness.m_mvp.m == 1)
	{
		copy_size = m_size_m;
	}
	else
	{
		LOG_FATAL("Invalid dirtiness state!");
		throw std::invalid_argument("Invalid dirtiness state.");
	}

	GET_CONTEXT;

	auto data = context->m_device.mapMemory(m_staging_buffer_memory, m_offset, copy_size);
	memcpy(data, &m_mvp, copy_size);
	context->m_device.unmapMemory(m_staging_buffer_memory);

	auto cmd_buffer = context->begin_single_use_command_buffer();
	cmd_buffer.copyBuffer(m_staging_buffer, m_buffer, { m_offset, m_offset, copy_size });
	context->end_single_use_command_buffer();

	m_dirtiness.m_dirty = 0;
}

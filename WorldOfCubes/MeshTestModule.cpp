#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>
#include "Fixtures.h"
#include "MeshBuffer.h"

BOOST_AUTO_TEST_SUITE(mesh_test_suite)

BOOST_AUTO_TEST_CASE(mesh_buffer_test)
{
	//Test normal use case of buffer.
	{
		MeshBuffer buffer;

		auto vertex_loc = buffer.add_buffer_element(MeshBufferElementFormat::eFloat, MeshBufferElementBits::e32, MeshBufferElementCount::e3);
		auto index_loc = buffer.add_buffer_element(MeshBufferElementFormat::eUnsignedInt, MeshBufferElementBits::e32, MeshBufferElementCount::e1);
		auto tex_coord_loc = buffer.add_buffer_element(MeshBufferElementFormat::eFloat, MeshBufferElementBits::e32, MeshBufferElementCount::e2);

		BOOST_CHECK_NE(vertex_loc, std::numeric_limits<uint32_t>::max());
		BOOST_CHECK_NE(index_loc, std::numeric_limits<uint32_t>::max());
		BOOST_CHECK_NE(tex_coord_loc, std::numeric_limits<uint32_t>::max());

		buffer.initialize();

		std::vector<float> vertex =
		{
			0.0f, 0.1f, 0.2f,
			1.0f, 1.1f, 1.2f,
			2.0f, 2.1f, 2.2f,

			3.0f, 3.1f, 3.2f,
			4.0f, 4.1f, 4.2f,
			5.0f, 5.1f, 5.2f,
		};

		std::vector<uint32_t> indexes =
		{
			1, 
			2, 
			3,
			4,
			5, 
			6,
		};

		std::vector<float> tex_coords =
		{
			0.0f, 0.0f,
			0.5f, 0.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.5f,
			1.0f, 1.0f,
		};

		BOOST_CHECK_EQUAL(buffer.set_buffer<float>(vertex_loc, vertex), 6);
		BOOST_CHECK_EQUAL(buffer.set_buffer<uint32_t>(index_loc, indexes), 6);
		BOOST_CHECK_EQUAL(buffer.set_buffer<float>(tex_coord_loc, tex_coords), 6);

		const auto& final_buf = buffer.get_buffer();
		final_buf.size();
	}
}

BOOST_AUTO_TEST_SUITE_END()
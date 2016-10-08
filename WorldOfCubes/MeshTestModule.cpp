#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>
#include "Fixtures.h"
#include "MeshBuffer.h"
#include "GraphicsSystem.h"
#include "ShaderSystem.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "Entity.h"
#include "ColoredMaterial.h"

BOOST_FIXTURE_TEST_SUITE(mesh_test_suite, BasicAppFixture)

BOOST_AUTO_TEST_CASE(mesh_buffer_test)
{
	SAFE_GET(engine, m_engine);
	SAFE_GET(graphics_system, engine->get_graphics_sytem());
	SAFE_GET(shader_system, graphics_system->get_shader_system());


	//Test normal use case of buffer.
	{
		auto material = graphics_system->get_material<ColoredMaterial>();
		auto entity = engine->create_entity();

		auto transform = entity->attach<TransformComponent>();
		auto mesh = entity->attach<MeshComponent>();

		mesh->set_material(material);

		std::vector<float> position =
		{
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.0f, 0.5f, 0.5f,
		};

		auto pos_count = mesh->set_buffer(ColoredMaterial::POSITION_LOCATION, position);

		std::vector<float> color =
		{
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.0f, 0.5f, 0.5f,
		};

		auto color_count = mesh->set_buffer(ColoredMaterial::COLOR_LOCATION, color);

		std::vector<uint16_t> indexes =
		{
			1, 2, 3
		};

		mesh->set_indexes(indexes);

		BOOST_CHECK_EQUAL(pos_count, color_count);
		BOOST_CHECK_EQUAL(pos_count, indexes.size());

		std::vector<char> intervealed;

		convert_buffer(std::vector<float>{
			-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
				0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
				0.0f, 0.5f, 0.5f, 0.0f, 0.5f, 0.5f,
		}, intervealed);

		auto buffer = mesh->get_buffer();

		BOOST_CHECK_EQUAL_COLLECTIONS(begin(buffer), end(buffer), begin(intervealed), end(intervealed));
	}
}

BOOST_AUTO_TEST_SUITE_END()
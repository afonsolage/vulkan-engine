#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "Entity.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "TestComponent.h"
#include <boost/mpl/list.hpp>

BOOST_AUTO_TEST_SUITE(entity_test_suite)

BOOST_AUTO_TEST_CASE(entity_test)
{
	//Check exception when using a entity without a shared pointer.
	{
		Entity to_fail_entity;

		BOOST_CHECK_THROW(to_fail_entity.attach<MeshComponent>(), std::bad_weak_ptr);
	}

	auto entity = std::make_shared<Entity>();

	//Test attach
	auto& component = entity->attach<MeshComponent>();
	BOOST_CHECK(component.is_attached());

	//Test get
	auto component_2 = entity->get_component<MeshComponent>();
	BOOST_CHECK(!component_2.expired());

	//Test detach
	auto component_3 = entity->detach<MeshComponent>();
	BOOST_CHECK(!component_3->is_attached());
	auto component_4 = entity->get_component<MeshComponent>();
	BOOST_CHECK(component_4.expired());
}

typedef boost::mpl::list<TestComponent, MeshComponent, TransformComponent> component_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(component_test_generic, T, component_types)
{
	//Test a simple usage.
	{
		auto entity = std::make_shared<Entity>();

		auto& component = entity->attach<T>();
		component.update();

		entity->detach<T>();
	}

	//Test a replacement usage
	{
		auto entity = std::make_shared<Entity>();

		auto& component = entity->attach<T>();
		auto& component2 = entity->attach<T>();
		component2.update();

		entity->detach<T>();
	}
}

BOOST_AUTO_TEST_SUITE_END()
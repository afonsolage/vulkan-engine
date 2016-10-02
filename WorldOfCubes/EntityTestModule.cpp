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
	auto entity2 = std::make_shared<Entity>();

	auto uid = entity->get_uid();

	//Test UID
	BOOST_CHECK(entity->get_uid() != 0);
	BOOST_CHECK(entity->get_uid() != entity2->get_uid());

	//Operator check
	BOOST_CHECK(*entity != *entity2);
	BOOST_CHECK(*entity == entity->get_uid());

	//Test attach
	auto component = entity->attach<MeshComponent>();
	BOOST_CHECK(component->is_attached());

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

		auto component = entity->attach<T>();
		component->update();

		entity->detach<T>();
	}

	//Test a replacement usage
	{
		auto entity = std::make_shared<Entity>();

		auto component = entity->attach<T>();
		auto sp_component = entity->get_component<T>().lock();

		auto component2 = entity->attach<T>();
		auto sp_component2 = entity->get_component<T>().lock();
		component2->update();

		//Unique id test
		BOOST_CHECK(sp_component->get_uid() != component2->get_uid());

		//Operators test
		BOOST_CHECK(*sp_component != *component2);
		BOOST_CHECK(*sp_component2 == *component2);
		BOOST_CHECK(*sp_component == *component);

		auto uid = sp_component->get_uid();
		auto uid2 = sp_component2->get_uid();

		BOOST_CHECK(*sp_component == uid);
		BOOST_CHECK(*sp_component != uid2);

		entity->detach<T>();
	}
}

BOOST_AUTO_TEST_CASE(transform_component_children_test)
{
	auto entity = std::make_shared<Entity>();

	auto component = entity->attach<TransformComponent>();
	uint64_t removed_uid;
	//Test valid transform child insertion
	{
		auto entity2 = std::make_shared<Entity>();
		auto component2 = entity2->attach<TransformComponent>();

		component->add(component2);

		BOOST_CHECK(component->get_child_count() == 1);
		BOOST_CHECK(!component2->get_parent().expired());
		removed_uid = component2->get_uid();
	}

	//Test auto remove on destruction
	{
		BOOST_CHECK(component->get_child_count() == 0);
	}

	auto child = std::make_shared<Entity>();
	auto child_component = child->attach<TransformComponent>();

	//Test exists method
	{
		component->add(child_component);

		BOOST_CHECK(component->exists(*child_component));
		BOOST_CHECK(!component->exists(removed_uid));
	}

	//Test remove method
	{
		component->remove(removed_uid);
		BOOST_CHECK(component->exists(*child_component));

		component->remove(child_component->get_uid());
		BOOST_CHECK(!component->exists(component->exists(*child_component)));
		BOOST_CHECK(child_component->is_attached());
		BOOST_CHECK(child_component->get_parent().expired());
	}

	//Test add a orphan component
	{
		child.reset();
		component->add(child_component);
		BOOST_CHECK(!component->exists(*child_component));

		child_component->add(component);
		BOOST_CHECK(component->get_parent().expired());
	}

	child_component.reset();
	//Test dual addition (parent -> child -> parent)
	{
		child = std::make_shared<Entity>();
		child_component = child->attach<TransformComponent>();

		component->add(child_component);
		BOOST_CHECK(component->exists(*child_component));

		child_component->add(component);
		BOOST_CHECK(!child_component->exists(*component));
	}

	//Test auto remove when adding to a new parent
	{
		child = std::make_shared<Entity>();
		child_component = child->attach<TransformComponent>();

		auto entity2 = std::make_shared<Entity>();
		auto component2 = entity2->attach<TransformComponent>();

		component->add(child_component);
		component2->add(child_component);

		BOOST_CHECK(!component->exists(*child_component));
		BOOST_CHECK(component2->exists(*child_component));
		BOOST_CHECK(child_component->get_parent().lock()->get_uid() == component2->get_uid());
	}
}

BOOST_AUTO_TEST_CASE(transform_component_local_translate_test)
{
	auto entity = std::make_shared<Entity>();
	auto component = entity->attach<TransformComponent>();

	//Test default values
	BOOST_CHECK(component->get_local_translate() == glmc::vec3_zero);
	BOOST_CHECK(component->get_local_scaling() == glmc::vec3_one);
	BOOST_CHECK(component->get_local_rotation() == glm::quat(glmc::vec3_zero));

	//Test simple translate
	component->set_local_translate({ 10, 10, 10 });
	BOOST_CHECK(component->get_local_translate() == glm::vec3(10, 10, 10));

	//Test simple rotation
	component->set_local_rotation_degrees({ 0, 10, 0 });

	//Teste simple scaling
	component->set_local_scaling({ 1.0f, 1.5f, 1.0f });
	BOOST_CHECK(component->get_local_scaling() == glm::vec3(1.0f, 1.5f, 1.0f));

	//Test combined translate
	component->add_local_translate({ -5, 5, 20 });
	BOOST_CHECK(component->get_local_translate() == glm::vec3(5, 15, 30));

	//Test combined rotation
	component->add_local_rotation_degrees({ 5, 10, 0 });
	BOOST_CHECK(almost_equals<glm::quat>(component->get_local_rotation(), glm::quat(glm::radians(glm::vec3(0, 10, 0))) * glm::quat(glm::radians(glm::vec3(5, 10, 0)))));

	//Test huge number of rotations
	glm::quat control = component->get_local_rotation();
	for (uint32_t i = 0; i < 10000; i++)
	{
		glm::vec3 euler(glm::linearRand(0.0f, glm::two_pi<float>()), glm::linearRand(0.0f, glm::two_pi<float>()), glm::linearRand(0.0f, glm::two_pi<float>()));
		component->add_local_rotation(euler);
		control *= glm::quat(euler);
	}
	BOOST_CHECK(almost_equals<glm::quat>(component->get_local_rotation(), control));

}

BOOST_AUTO_TEST_SUITE_END()
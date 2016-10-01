#pragma once

class AbstractComponent;

class Entity : public std::enable_shared_from_this<Entity>
{
public:
	Entity();
	virtual ~Entity();

	void update();

	template<typename T, typename... Args>
	T& attach(Args&&... args);
	template<typename T>
	std::shared_ptr<T> detach();
	template<typename T>
	std::weak_ptr<T> get_component();

private:
	std::vector<std::shared_ptr<AbstractComponent>> m_components;
};

template<typename T, typename... Args>
inline T& Entity::attach(Args&&... args)
{
	//Detach any previous component, if it exists.
	detach<T>();

	//Create a new instance of component and perfect-forward the constructor arguments.
	auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
	T& result = *ptr;
	//Tell component it was attached on this entity and pass a shared pointer to it.

#ifdef NDEBUG
	result.attached(shared_from_this());
#else
	try
	{
		result.attached(shared_from_this());
	}
	catch (std::bad_weak_ptr ex)
	{
		LOG_FATAL("Entity must be a shared object!.");
		throw;
	}
#endif

	//Move the new contructed pointer to components list.
	m_components.push_back(std::move(ptr));
	return result;
}

template<typename T>
inline std::shared_ptr<T> Entity::detach()
{
	//Try to get an iterator of T component
	auto it = std::remove_if(begin(m_components), end(m_components), [&tpid = typeid(T)](const std::shared_ptr<AbstractComponent>& ptr)
	{
		return tpid == typeid(*ptr);
	});

	if (it != m_components.end())
	{
		std::shared_ptr<T> result = std::shared_ptr<T>(std::static_pointer_cast<T>(*it));

		m_components.erase(it);

		result->detached();

		return result;
	}

	return std::shared_ptr<T>();
}

template<typename T>
inline std::weak_ptr<T> Entity::get_component()
{
	auto it = std::find_if(begin(m_components), end(m_components), [&tpid = typeid(T)](const std::shared_ptr<AbstractComponent>& ptr)
	{
		return tpid == typeid(*ptr);
	});

	if (it != m_components.end())
	{
		return std::weak_ptr<T>(std::static_pointer_cast<T>(*it));
	}

	return std::weak_ptr<T>();
}
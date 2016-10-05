#pragma once

class Entity;
class CameraComponent;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
	Scene();
	virtual ~Scene();

	void add(std::shared_ptr<Entity> entity);
	void update();

	void set_dirty() { m_dirty = true; }

	std::weak_ptr<CameraComponent> get_camera() { return m_camera; }
protected:
	void remove_dead_entities();

	std::weak_ptr<CameraComponent> m_camera;
	std::vector<std::weak_ptr<Entity>> m_entities;

	bool m_dirty;
};
#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include<algorithm>
#include<bitset>
#include<array>
#include <mutex>

using namespace std;

class Component;
class Entity;

using ComponentID = size_t;


inline ComponentID getComponentTypeID() {
	static ComponentID lastID = 0;
	return lastID += 1;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

constexpr size_t maxComponents = 32;

using ComponentBitSet = bitset<maxComponents>;
using CompoenentArray = array<Component*, maxComponents>;

class Component {
private:
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	void setReady(bool){}
	bool getReady(){}


	virtual ~Component(){}
};

class Entity {
private:

	bool active = true;
	bool ready = false;
	vector<unique_ptr<Component>> components;

	CompoenentArray componentArray;
	ComponentBitSet componentBitSet;
	 
public:

	void update() {
		for (auto& c : components)
				c->update();
	}

	void draw() {
		for (auto& c : components)
				c->draw();
	}

	bool isActive() { return this->active; }

	void destroy() { active = false; }


	template <typename T> bool hasComponent() const {
		return componentBitSet(getComponentTypeID<T>());
	}

	template <typename T, typename... TArgs> T& addComponent(TArgs&& ... mArgs) {
		T* c(new T(forward<TArgs>(mArgs)...));
		c->entity = this;
		unique_ptr<Component> uPtr{ c };
		components.emplace_back(move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template <typename T> T& getComponent() const {
		auto ptr(componentArray[getComponentTypeID<T>()]);
		
		return *static_cast<T*>(ptr);
	}
};

class Manager {
private:
	vector<unique_ptr<Entity>> entities;
	mutex m;
public:
	void update() {
		m.lock();
		for (auto& e : entities)
			if (e != nullptr)
				e->update();
		m.unlock();
	}

	void draw() {
		m.lock();
		for (auto& e : entities)
			if (e != nullptr)
				e->draw();
		m.unlock();
	}

	void refresh() {
	/*	entities.erase(remove_if(begin(entities), end(entities), [](const unique_ptr<Entity> & mEntity) {
			return !mEntity->isActive();
		}), end(entities));*/
	}

	

	Entity& addEntity() {
		Entity* e = new Entity();
		unique_ptr<Entity> uPtr{ e };
			entities.emplace_back(move(uPtr));

		return *e;
	}
	
	void addEntity(Entity* e) {
		m.lock();
		unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(move(uPtr));
		m.unlock();
	}
};


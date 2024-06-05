#include "ECS.h"

#pragma region Entity

Entity::Entity(const char* name, const Vector2f& localPosition) : name(name)
{
	m_transform = &AddComponent<Transform>(localPosition);
}

void Entity::Start()
{
	for (auto& c : components)
	{
		c->m_owner = this;
		c->m_transform = m_transform;
		c->Start();
	}
	for (size_t i = 0; i < m_transform->children.size(); i++) m_transform->children[i]->owner->Start();
}

void Entity::Update(const float _deltaTime)
{
	for (auto& c : components) c->Update(_deltaTime);
	for (size_t i = 0; i < m_transform->children.size(); i++) m_transform->children[i]->owner->Update(_deltaTime);
}

void Entity::FixedUpdate(const float _fixedDeltaTime)
{
	for (auto& c : components) c->FixedUpdate(_fixedDeltaTime);
	for (size_t i = 0; i < m_transform->children.size(); i++) m_transform->children[i]->owner->FixedUpdate(_fixedDeltaTime);
}

void Entity::Draw()
{
	for (auto& c : components) c->Draw();
	for (size_t i = 0; i < m_transform->children.size(); i++) m_transform->children[i]->owner->Draw();
}

#pragma endregion

#pragma region ECSManager
void EntityManager::PreUpdate()
{
	for (size_t i = 0; i < pendingEntities.size(); i++)
	{
		Entity* handle = pendingEntities.front();
		pendingEntities.pop();
		if (handle != nullptr && (!handle->IsSelfActive() || handle->m_transform->parent != NULL))
		{
			continue;
		}
		handle->Start();
	}
}

void EntityManager::Update(const float _deltaTime)
{
	for (auto& e : entities)
		if (e->m_transform->parent == nullptr && e->IsSelfActive()) e->Update(_deltaTime);
}

void EntityManager::FixedUpdate(const float _fixedDeltaTime)
{
	for (auto& e : entities)
		if (e->m_transform->parent == nullptr && e->IsSelfActive()) e->FixedUpdate(_fixedDeltaTime);
}

void EntityManager::Draw()
{
	for (auto& e : entities)
		if (e->m_transform->parent == nullptr && e->IsSelfActive())
			e->Draw();
}

/// <summary>
/// Removes inactive entities
/// </summary>
void EntityManager::Refresh()
{
	entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		[](const std::unique_ptr<Entity>& mEntity)
		{
			return mEntity->destroyed;
		}), std::end(entities));
}

void EntityManager::RemoveChild(Transform* child)
{
	entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		[child](const std::unique_ptr<Entity>& e)
		{
			return e->m_transform == child;
		}));
}
#pragma endregion

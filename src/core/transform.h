#pragma once

#include "../core/math/vectormath.hpp"
#include "../ECS/ECS.h"
#include <SDL_ttf.h>

/// <summary>
/// Component which gives position logic along with helping methods
/// </summary>
class Transform final : public Component
{
	friend class Entity;

public:
	Transform(const Vector2f& localPosition);

	// TODO: Probably add rotation
	Vector2f localPosition;
	Vector2f localScale;
	__property(getPosition, setPosition) Vector2f position;
	__property(getLossyScale) Vector2f lossyScale;
	__property(getParent, setParent) Transform* parent;
	__property(getRoot) Transform* root;

	void Start() override
	{
		updateGlobalPosition();
	}

#pragma region Property Functions

	const Vector2f& getPosition()
	{
		if (hasChanged) {
			updateGlobalPosition();
		}
		return globalPosition;
	}

	FORCEINLINE void setPosition(const Vector2f& position)
	{
		if (m_parent == nullptr) {
			localPosition = position;
		}
		else
			localPosition = position - m_parent->getPosition();

		hasChanged = true;
	}

	FORCEINLINE void setPosition(float x = 0.f, float y = 0.f)
	{
		Vector2f v;
		v.Set(x, y);
		setPosition(v);
	}

	FORCEINLINE const Vector2f getLossyScale() const
	{
		if (m_parent == nullptr)
			return localScale;
		return m_parent->lossyScale.Scale(localScale);
	}

	FORCEINLINE void setParent(Transform* parent, bool worldPositionStays = false)
	{
		if (parent == nullptr && m_parent == nullptr) return;
		if (worldPositionStays)
			localPosition = position;

		if (parent == nullptr)
			m_parent->children.erase(std::find(m_parent->children.begin(), m_parent->children.end(), this));
		else parent->children.push_back(this);
		this->m_parent = parent;
	}

	FORCEINLINE Transform* getRoot()
	{
		if (parent == nullptr) return this;
		Transform* current = this;
		Transform* prev = nullptr;
		while (current != nullptr)
		{
			prev = current;
			current = prev->parent;
		}
		return prev;
	}

	FORCEINLINE Transform* getParent() const
	{
		return m_parent;
	}

	FORCEINLINE bool getHasChanged() const {
		return hasChanged;
	}

	FORCEINLINE size_t getChildCount() const
	{
		return children.size();
	}

	FORCEINLINE Transform* getChildAt(size_t i) const
	{
		return children.at(i);
	}

#pragma endregion

private:
	std::vector<Transform*> children;
	Transform* m_parent;
	Vector2f globalPosition;
	bool hasChanged = true;

	void updateGlobalPosition()
	{
		if (m_parent == nullptr) {
			globalPosition = localPosition;
		}
		else {
			globalPosition = m_parent->getPosition() + Vector2f::Scale(localPosition, lossyScale);
		}

		hasChanged = false;
		for (Transform* child : children) {
			child->parentChanged();
		}
	}

	void parentChanged()
	{
		hasChanged = true;
		for (Transform* child : children) {
			child->parentChanged();
		}
	}
};
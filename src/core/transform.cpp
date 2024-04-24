#include "transform.h"

Transform::Transform(const Vector2f& localPosition) :
	localPosition(localPosition),
	localScale(1.f, 1.f),
	m_parent(nullptr)
{
	hasChanged = true;
}
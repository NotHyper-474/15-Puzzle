
template<typename T> requires std::is_base_of<Component, T>::value
T* Game::FindEntityOfType() const
{
	auto it = std::find_if(ecsManager.entities.begin(), ecsManager.entities.end(), [](const std::unique_ptr<Entity>& e) { return e->HasComponent<T>(); });

	if (it != ecsManager.entities.end())
	{
		return (*it->get()).template GetComponent<T>();
	}

	return nullptr;
}

template<typename T> requires std::derived_from<T, Component>
std::vector<T*> Game::FindEntitiesOfType() const
{
	std::vector<T*> result;
	for (auto& ent : ecsManager.entities)
	{
		T* c = nullptr;
		if (ent->TryGetComponent<T>(&c))
			result.push_back(c);
	}
	return result;
}

template<typename T, size_t L> requires std::derived_from<T, Component>
int Game::FindEntitiesOfTypeNonAlloc(std::array<T*, L>& result)
{
	size_t i = 0;
	for (auto& ent : ecsManager.entities)
	{
		T* c = nullptr;
		if (ent->TryGetComponent<T>(&c))
		{
			result[++i] = c;
			if (i == L) break;
		}
	}
	return i;
}
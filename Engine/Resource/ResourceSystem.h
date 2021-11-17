#pragma once
#include "Framework/System.h"
#include "Resource.h"
#include <string>
#include <map>
#include <memory>
#include <algorithm>

namespace nEngine
{
	class ResourceSystem : public System
	{
	public:
		virtual void Startup() override {}
		virtual void Shutdown() override {}
		virtual void Update(float dt) override {}

		void Add(const std::string& name, std::shared_ptr<Resource> resource);

		template <typename T>
		std::shared_ptr<T> Get(const std::string& name, void* data = nullptr);

	private:
		std::map<std::string, std::shared_ptr<Resource>> resources;
	};

	template<typename T>
	inline std::shared_ptr<T> ResourceSystem::Get(const std::string& name, void* data)
	{
		if (resources.find(string_tolower(name)) != resources.end())
		{
			return std::dynamic_pointer_cast<T>(resources[string_tolower(name)]);
		}
		else
		{
			std::shared_ptr resource = std::make_shared<T>();
			resource->Load(name, data);
			resources[string_tolower(name)] = resource;

			return resource;
		}
	}

	inline void ResourceSystem::Add(const std::string& name, std::shared_ptr<Resource> resource)
	{
		resources[string_tolower(name)] = resource;
	}
}
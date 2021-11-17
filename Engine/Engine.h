#pragma once

#define REGISTER_CLASS(class) nEngine::ObjectFactory::Instance().Register<class>(#class)


// core
#include "Core/Utilities.h"
#include "Core/FileSystem.h"
#include "Core/Timer.h"
#include "Core/Json.h"
#include "Core/Serializable.h"

// framework
#include "Framework/EventSystem.h"
#include "Framework/Singleton.h"
#include "Framework/Factory.h"

// math
#include "Math/Random.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"

// audio
#include "Audio/AudioSystem.h"

// input
#include "Input/InputSystem.h"

// graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"

// resource
#include "Resource/ResourceSystem.h"

// objects
#include "Object/Actor.h"
#include "Object/Scene.h"

// components
#include "Component/PhysicsComponent.h"
#include "Component/AudioComponent.h"

#include <vector>
#include <memory>
#include <algorithm>

namespace nEngine
{
	using ObjectFactory = Singleton<Factory<std::string, Object>>;

	class Engine
	{
	public:
		void Startup();
		void Shutdown();

		void Update();
		void Draw(Renderer* renderer);

		template<typename T>
		T* Get();

	public:
		FrameTimer time;

	private:
		std::vector<std::unique_ptr<System>> systems;
	};

	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems)
		{
			if (dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get());
		}

		return nullptr;
	}
}
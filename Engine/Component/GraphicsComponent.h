#pragma once
#include "Component.h"

namespace nEngine
{
	class Renderer;

	class GraphicsComponent : public Component
	{
	public:
		virtual void Draw(Renderer* renderer) = 0;

	};
}
#pragma once
#include "Renderer.h"
#include "Resource/Resource.h"
#include "Math/MathTypes.h"
#include <SDL.h>

namespace nEngine
{
	class Texture : public Resource
	{
	public:
		~Texture();
		bool Load(const std::string& name, void* data) override;
		
		void Bind() { glActiveTexture(unit); glBindTexture(target, texture); }
		bool CreateTexture(const std::string& filename, GLenum target = GL_TEXTURE_2D, GLuint unit = GL_TEXTURE0);

		static void FlipSurface(SDL_Surface* surface);

	protected:
		GLenum target{ GL_TEXTURE_2D };
		GLuint unit{ GL_TEXTURE0 };
		GLuint texture{ 0 };
	};
}
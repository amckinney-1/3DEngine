#include "Engine.h"
#include <glad\glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

// vertices
const float vertices[] =
{
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
};

const GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

int main(int argc, char** argv)
{
	nEngine::Engine engine;
	engine.Startup();
	engine.Get<nEngine::Renderer>()->Create("OpenGL", 800, 600);

	nEngine::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nEngine::SetFilePath("../resources");

	std::shared_ptr<nEngine::Program> program = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::Program>("basic_program");
	std::shared_ptr<nEngine::Shader> vshader = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::Shader>("shaders/basic.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<nEngine::Shader> fshader = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::Shader>("shaders/basic.frag", (void*)GL_FRAGMENT_SHADER);

	program->AddShader(vshader);
	program->AddShader(fshader);
	program->Link();
	program->Use();

	std::shared_ptr<nEngine::VertexIndexBuffer> vertexBuffer = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::VertexIndexBuffer>("vertex_index_buffer");
	vertexBuffer->CreateVertexBuffer(sizeof(vertices), 4, (void*)vertices);
	vertexBuffer->CreateIndexBuffer(GL_UNSIGNED_INT, 6, (void*)indices);
	vertexBuffer->SetAttribute(0, 3, 8 * sizeof(GLfloat), 0);
	vertexBuffer->SetAttribute(1, 3, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat));
	vertexBuffer->SetAttribute(2, 2, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat));

	// texture
	nEngine::Texture texture;
	texture.CreateTexture("textures/llama.jpg");
	texture.Bind();

	// uniform
	float time = 0;
	glm::vec3 tint{ 1.0, 0.1, 0.75 };

	program->SetUniform("scale", time);
	program->SetUniform("tint", tint);

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		SDL_PumpEvents();
		engine.Update();

		time += engine.time.deltaTime;

		program->SetUniform("scale", std::sin(time * 2));

		engine.Get<nEngine::Renderer>()->BeginFrame();

		vertexBuffer->Draw();

		engine.Get<nEngine::Renderer>()->EndFrame();
	}

	return 0;
}

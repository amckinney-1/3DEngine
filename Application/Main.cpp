#include "Engine.h"
#include <glad\glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

const float vertices[] =
{
	// front
	-1.0f, -1.0f,  1.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	// back
	-1.0f, -1.0f, -1.0, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, -1.0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, -1.0, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
};

const GLuint indices[] =
{
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

int main(int argc, char** argv)
{
	// create engine
	nEngine::Engine engine;
	engine.Startup();
	engine.Get<nEngine::Renderer>()->Create("OpenGL", 800, 600);

	// create scene
	std::unique_ptr<nEngine::Scene> scene = std::make_unique<nEngine::Scene>();
	scene->engine = &engine;

	nEngine::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nEngine::SetFilePath("../resources");

	// create shaders
	std::shared_ptr<nEngine::Program> program = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::Program>("basic_shader");
	std::shared_ptr<nEngine::Shader> vshader = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::Shader>("shaders/basic.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<nEngine::Shader> fshader = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::Shader>("shaders/basic.frag", (void*)GL_FRAGMENT_SHADER);
	program->AddShader(vshader);
	program->AddShader(fshader);
	program->Link();
	program->Use();

	// vertex buffer
	std::shared_ptr<nEngine::VertexBuffer> vertexBuffer = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::VertexBuffer>("cube_mesh");
	vertexBuffer->CreateVertexBuffer(sizeof(vertices), 4, (void*)vertices);
	vertexBuffer->CreateIndexBuffer(GL_UNSIGNED_INT, 36, (void*)indices);
	vertexBuffer->SetAttribute(0, 3, 8 * sizeof(GLfloat), 0);
	vertexBuffer->SetAttribute(1, 3, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat));
	vertexBuffer->SetAttribute(2, 2, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat));

	// texture
	auto texture = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::Texture>("textures/llama.jpg");
	texture->Bind();

	/*auto texture = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::Texture>("textures/rocks.bmp");
	texture->Bind();*/

	/*auto texture = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::Texture>("textures/wood.png");
	texture->Bind();*/

	// create camera
	{
		auto actor = nEngine::ObjectFactory::Instance().Create<nEngine::Actor>("Actor");
		actor->name = "camera";
		actor->transform.position = glm::vec3{ 0, 0, 10 };

		{
			auto component = nEngine::ObjectFactory::Instance().Create<nEngine::CameraComponent>("CameraComponent");
			component->SetPerspective(45.0f, 800.0f / 600.0f, 0.01f, 100.0f);
			actor->AddComponent(std::move(component));
		}

		{
			auto component = nEngine::ObjectFactory::Instance().Create<nEngine::FreeCameraController>("FreeCameraController");
			component->speed = 3;
			component->sensitivity = 0.1f;
			actor->AddComponent(std::move(component));
		}

		scene->AddActor(std::move(actor));
	}

	// create cube
	{
		auto actor = nEngine::ObjectFactory::Instance().Create<nEngine::Actor>("Actor");
		actor->name = "cube";
		actor->transform.position = glm::vec3{ 0, 0, 0 };

		auto component = nEngine::ObjectFactory::Instance().Create<nEngine::MeshComponent>("MeshComponent");
		component->program = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::Program>("basic_shader");
		component->vertexBuffer = engine.Get<nEngine::ResourceSystem>()->Get<nEngine::VertexBuffer>("cube_mesh");

		actor->AddComponent(std::move(component));
		scene->AddActor(std::move(actor));
	}

	glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 2 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
	program->SetUniform("view", view);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	program->SetUniform("projection", projection);

	glm::vec3 translate{ 0.0f };
	float angle{ 0 };

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
		scene->Update(engine.time.deltaTime);

		// update actor
		auto actor = scene->FindActor("cube");
		if (actor != nullptr)
		{
			actor->transform.rotation.y += engine.time.deltaTime;
		}

		engine.Get<nEngine::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine.Get<nEngine::Renderer>()->EndFrame();
	}

	return 0;
}

#include "Engine.h"
#include <glad/glad.h>
#include <sdl.h>

int main(int argc, char** argv)
{
	// create engine
	std::unique_ptr<nEngine::Engine> engine = std::make_unique<nEngine::Engine>();
	engine->Startup();
	engine->Get<nEngine::Renderer>()->Create("OpenGL", 800, 600);

	nEngine::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nEngine::SetFilePath("../resources");

	// create scene
	std::unique_ptr<nEngine::Scene> scene = std::make_unique<nEngine::Scene>();
	scene->engine = engine.get();

	// load scene
	rapidjson::Document document;
	bool success = nEngine::json::Load("scenes/main.scn", document);
	scene->Read(document);

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
		engine->Update();
		scene->Update(engine->time.deltaTime);

		//// update actor
		//auto actor = scene->FindActor("model");
		//if (actor != nullptr)
		//{
		//	actor->transform.rotation.y += engine->time.deltaTime;
		//}

		 // update actor
		auto actor = scene->FindActor("light");
		if (actor != nullptr)
		{
			glm::mat3 rotation = glm::rotate(engine->time.deltaTime, glm::vec3{ 0, 0, 1 });
			actor->transform.position = actor->transform.position * rotation;
		}

		engine->Get<nEngine::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine->Get<nEngine::Renderer>()->EndFrame();
	}

	return 0;
}

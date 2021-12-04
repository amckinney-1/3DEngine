#include "Material.h"
#include "Engine.h"

namespace nEngine
{
	bool Material::Load(const std::string& filename, void* data)
	{
		// cast the data void* to an Engine*
		auto engine = static_cast<Engine*>(data);

		// load the json file
		rapidjson::Document document;
		bool success = nEngine::json::Load(filename, document);
		if (!success)
		{
			SDL_Log("Could not load material file (%s).", filename.c_str());
			return false;
		}

		// color values
		JSON_READ(document, ambient);
		JSON_READ(document, diffuse);
		JSON_READ(document, specular);
		JSON_READ(document, shininess);

		// program
		std::string shader_name;
		JSON_READ(document, shader_name);
		shader = engine->Get<nEngine::ResourceSystem>()->Get<nEngine::Program>(shader_name, engine);

		// textures
		std::vector<std::string> texture_names;
		JSON_READ(document, texture_names);

		for (auto& name : texture_names)
		{
			auto texture = engine->Get<nEngine::ResourceSystem>()->Get<nEngine::Texture>(name, engine);
			if (texture.get()) // check for valid texture
				AddTexture(texture);
		}

		return true;

	}

	void Material::Set()
	{
		// set the shader (bind)
		shader->Use();
		// update shader material properties
		shader->SetUniform("material.ambient", ambient);
		shader->SetUniform("material.ambient", diffuse);
		shader->SetUniform("material.ambient", specular);
		shader->SetUniform("material.ambient", shininess);

		// set the textures (bind)
		Bind();
	}

	void Material::Bind()
	{
		for (auto& texture : textures)
			texture->Bind();
	}

	void Material::SetProgram(Program& program)
	{
		program.Use();
	}
}
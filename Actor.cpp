#include "Actor.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "TextureLoader.h"
#include <math.h> 
#include "MyArknightsScriptScript.h"

Actor::Actor(std::wstring& actorName, int actorTexture)
{
	texture = static_cast<unsigned int>(actorTexture);
	unloading = false;
	loading = true;
	transparency = 0.0f;
	reflect = false;
	brightness = 1.0f;
	transform = glm::mat4(1.0f);
	name = actorName;
}

void Actor::paint()
{
	if (loading) {
		transparency += 0.05f;
		if (transparency >= 1.0f) {
			loading = false;
		}
	}

	if (unloading && transparency > 0.0f) {
		transparency -= 0.05f;
		if (transparency <= 0.0f) {
			unloading = false;
		}
	}

	MyArknightsScriptScript::loader->setUniformData("brightness", brightness);
	MyArknightsScriptScript::loader->setUniformData("transparency", transparency);
	MyArknightsScriptScript::loader->setUniformData("transform", transform);

	if (reflect) {
		setTextureVertex({
			{1.0f, 1.0f},
			{0.0f, 1.0f},
			{1.0f, 0.0f},
			{0.0f, 0.0f}
		});
	}

	applyVertex();
	applyTexture();
	callOpenGL();
}

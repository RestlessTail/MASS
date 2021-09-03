#include "Retreat.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MyArknightsScriptScript.h"

Retreat::Retreat()
{
	x = 0.0f;
	actorPos = 'L';
	disabled = true;
	finished = false;
}

void Retreat::paint()
{
	float transparency = 1.0f;
	float offsetY = 0.0f;

	transparency = 1.0f - x;
	offsetY = -0.15f * x;

	if (actorPos == 'L') {
		MyArknightsScriptScript::ActiveCharacterL->transparency = transparency;
		MyArknightsScriptScript::ActiveCharacterL->transform = glm::translate(
			glm::mat4(1.0f),
			glm::vec3(0.0f, offsetY, 0.0f)
		);
	}
	else {
		MyArknightsScriptScript::ActiveCharacterR->transparency = transparency;
		MyArknightsScriptScript::ActiveCharacterR->transform = glm::translate(
			glm::mat4(1.0f),
			glm::vec3(0.0f, offsetY, 0.0f)
		);
	}

	x += 0.05f;
	if (x >= 1.0f) {
		finished = true;
		if (actorPos == 'L') {
			delete MyArknightsScriptScript::ActiveCharacterL;
			MyArknightsScriptScript::ActiveCharacterL = nullptr;
		}
		else {
			delete MyArknightsScriptScript::ActiveCharacterR;
			MyArknightsScriptScript::ActiveCharacterR = nullptr;
		}
	}
}

void Retreat::enable(char c)
{
	x = 0.0f;
	disabled = false;
	finished = false;
	actorPos = c;
}

void Retreat::disable()
{
	disabled = true;
}

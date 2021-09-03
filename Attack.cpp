#include "Attack.h"
#include "MyArknightsScriptScript.h"
#include "Mask.h"
#include <math.h>
#include "Actor.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Attack::Attack()
{
	x = -1.0f;
	disabled = true;
	finished = false;
	actorPos = 'L';
}

void Attack::paint()
{
	float brightness = 1.0f; //´¢´æÕÚÕÖ²ãµÄÁÁ¶È
	float offsetX = 0.0f; //½ÇÉ«Æ«ÒÆÁ¿

	brightness = 0.9f - x * x;
	MyArknightsScriptScript::mask->setColor(1.0f, 1.0f, 1.0f, brightness);

	if (actorPos == 'L') {
		offsetX = 0.1f * sin(12.566f * (x - 0.9f));//12.566 = 4 * 3.1415
		MyArknightsScriptScript::ActiveCharacterL->transform = glm::translate(
			glm::mat4(1.0f),
			glm::vec3(offsetX, 0.0f, 0.0f)
		);
	}
	else {
		offsetX = -0.1f * sin(12.566f * (x - 0.9f));//12.566 = 4 * 3.1415
		MyArknightsScriptScript::ActiveCharacterR->transform = glm::translate(
			glm::mat4(1.0f),
			glm::vec3(offsetX, 0.0f, 0.0f)
		);
	}
	
	x += 0.1f;
	if (x >= 1.0f) {
		finished = true;
	}
}

void Attack::enable(char c)
{
	x = -1.0f;
	disabled = false;
	finished = false;
	actorPos = c;
}

void Attack::disable()
{
	disabled = true;
}

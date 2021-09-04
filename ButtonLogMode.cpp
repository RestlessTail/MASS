#include "ButtonLogMode.h"
#include "MyArknightsScriptScript.h"
#include "LogView.h"
#include "Shadow.h"
#include "ButtonBack.h"
#include "Mask.h"

ButtonLogMode::ButtonLogMode()
{
}

void ButtonLogMode::onClicked()
{
	MyArknightsScriptScript::logView->visible = true;
	MyArknightsScriptScript::logView->allowScroll = true;
	MyArknightsScriptScript::buttonBack->visible = true;
	MyArknightsScriptScript::mask->visible = true;
	MyArknightsScriptScript::mask->setColor(0.0f, 0.0f, 0.0f, 0.8f);

	for (auto b : MyArknightsScriptScript::buttonGroup) {
		b->visible = false;
	}

	disabled = true;
	MyArknightsScriptScript::buttonBack->disabled = false;
	MyArknightsScriptScript::buttonBack->connectedButton = this;
}

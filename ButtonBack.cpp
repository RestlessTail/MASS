#include "ButtonBack.h"
#include "MyArknightsScriptScript.h"
#include "LogView.h"
#include "Shadow.h"
#include "Mask.h"

ButtonBack::ButtonBack()
{
	connectedButton = nullptr;
}

void ButtonBack::onClicked()
{
	MyArknightsScriptScript::logView->visible = false;
	MyArknightsScriptScript::logView->allowScroll = false;
	MyArknightsScriptScript::shadow->visible = true;
	MyArknightsScriptScript::mask->visible = false;

	visible = false;

	for (auto b : MyArknightsScriptScript::buttonGroup) {
		b->visible = true;
	}

	disabled = true;
	connectedButton->disabled = false;
}

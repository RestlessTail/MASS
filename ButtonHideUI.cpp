#include "ButtonHideUI.h"
#include "MyArknightsScriptScript.h"
#include "Shadow.h"
#include "Line.h"

ButtonHideUI::ButtonHideUI()
{
	hide = false;
}

void ButtonHideUI::onClicked()
{
	MyArknightsScriptScript::shadow->visible = hide;
	MyArknightsScriptScript::lineControl->visible = hide;
	MyArknightsScriptScript::nameControl->visible = hide;

	for (auto b : MyArknightsScriptScript::buttonGroup) {
		b->visible = hide;
	}

	if (hide) {
		useDefaultResponseRange();
	}
	else {
		setResponseRange(
			{ -1.0f, 1.0f, 0.0f },
			{ 1.0f, 1.0f, 0.0f },
			{ -1.0f, -1.0f, 0.0f },
			{ 1.0f, -1.0f, 0.0f }
		);
	}

	hide = !hide;
}

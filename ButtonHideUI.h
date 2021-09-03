#pragma once
#include "Button.h"
class ButtonHideUI : public Button{
public:
	ButtonHideUI();
	void onClicked() override;
	bool hide;
};


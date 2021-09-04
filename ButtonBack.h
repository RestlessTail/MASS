#pragma once
#include "Button.h"

class ButtonBack : public Button{
public:
	ButtonBack();
	void onClicked() override;
	Button* connectedButton;
};


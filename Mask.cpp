#include "Mask.h"
#include "MyArknightsScriptScript.h"

Mask::Mask()
{
	visible = false;
	r = 1.0f;
	g = 1.0f;
	b = 1.0f;
	a = 1.0f;
}

void Mask::setColor(float R, float G, float B, float A)
{
	r = R;
	g = G;
	b = B;
	a = A;
}

void Mask::paint()
{
	MyArknightsScriptScript::loader->SetActiveProgram(3);
	//MyArknightsScriptScript::loader->setUniformData("MaskColor", 0.0f, 0.0f, 0.0f, 0.8f);
	MyArknightsScriptScript::loader->setUniformData("MaskColor", r, g, b, a);

	applyVertex();
	applyTexture();
	callOpenGL();
}

#include "Scene.h"
#include "MyArknightsScriptScript.h"
#include <iostream>
#include <windows.h>
//#include <gl/GL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Scene::Scene(unsigned int t)
{
	setupVertex({ -1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f });
	unloading = false;
	loading = true;
	transparency = 0.0f;
	texture = t;
}

void Scene::paint(){

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

	MyArknightsScriptScript::loader->setUniformData("transparency", transparency);
	
	applyVertex();
	applyTexture();
	callOpenGL();
}

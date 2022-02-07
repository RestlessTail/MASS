#include "CPPInterface.h"
#include "MyArknightsScriptScript.h"
#include "CharacterController.h"
#include "TextureLoader.h"
#include "Action.h"

MyArknightsScriptScript* engine;

void MASS::InitEngine()
{
	engine = new MyArknightsScriptScript;
	CharacterControllerInit();
}

void MASS::PlayAction(MASSAction::Action* action)
{
	MyArknightsScriptScript::running = true;
	engine->curAction = action;
	engine->exec();
	delete action;
}

int MASS::LoadTextureResource(const char* filename)
{
	unsigned int id;
	TextureLoader::loadTexture(&id, filename);
	return static_cast<int>(id);
}

int MASS::LoadSoundResource(const char* filename)
{
	int id = MyArknightsScriptScript::AllSoundSource.size();
	SoundSource_t* ssp = MyArknightsScriptScript::irrklangSoundEngine->addSoundSourceFromFile(filename);
	MyArknightsScriptScript::AllSoundSource.emplace(id, ssp);
	return id;
}

void MASS::FreeTextureResource(unsigned int id)
{
	TextureLoader::freeTexture(id);
}

void MASS::FreeSoundResource(int id)
{
	auto itr = MyArknightsScriptScript::AllSoundSource.find(id);
	if (itr == MyArknightsScriptScript::AllSoundSource.end()) {
		return;
	}
	MyArknightsScriptScript::irrklangSoundEngine->removeSoundSource(itr->second);
	MyArknightsScriptScript::AllSoundSource.erase(itr);
}

int MASS::GenerateActor(const wchar_t * name)
{
	Actor_t* a = new Actor_t;
	a->name = name;
	int id = MyArknightsScriptScript::AllActor.size();
	MyArknightsScriptScript::AllActor.emplace(id, a);
	return id;
}

void MASS::AddTextureForActor(int actor, const wchar_t * key, int texture)
{
	auto itr = MyArknightsScriptScript::AllActor.find(actor);
	if (itr == MyArknightsScriptScript::AllActor.end()) {
		return;
	}
	itr->second->textureMap.emplace(key, texture);
}

void MASS::SetActiveTextureForActor(int actor, const wchar_t * key)
{
	auto itr = MyArknightsScriptScript::AllActor.find(actor);
	if (itr == MyArknightsScriptScript::AllActor.end()) {
		return;
	}
	auto textureItr = itr->second->textureMap.find(key);
	if (textureItr == itr->second->textureMap.end()) {
		return;
	}
	itr->second->activeTexture = textureItr->second;

	std::wstring name = itr->second->name;
	if (MyArknightsScriptScript::ActiveCharacterL) {
		if (name == MyArknightsScriptScript::ActiveCharacterL->name) {
			MyArknightsScriptScript::ActiveCharacterL->texture = itr->second->activeTexture;
		}
	}
	if (MyArknightsScriptScript::ActiveCharacterR) {
		if (name == MyArknightsScriptScript::ActiveCharacterR->name) {
			MyArknightsScriptScript::ActiveCharacterR->texture = itr->second->activeTexture;
		}
	}
}

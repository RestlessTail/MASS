#pragma once
#include "Action.h"
#include "Sound.h"

namespace MASS {
	__declspec(dllexport) void InitEngine();
	__declspec(dllexport) void PlayAction(MASSAction::Action* action);
	__declspec(dllexport) int LoadTextureResource(const char* filename);
	__declspec(dllexport) int LoadSoundResource(const char* filename);
	__declspec(dllexport) void FreeTextureResource(unsigned int id);
	__declspec(dllexport) void FreeSoundResource(int id);
	__declspec(dllexport) int GenerateActor(const wchar_t * name);
	__declspec(dllexport) void AddTextureForActor(int actor, const wchar_t * key, int texture);
	__declspec(dllexport) void SetActiveTextureForActor(int actor, const wchar_t * key);
}
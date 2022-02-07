#include "CPPInterface.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "CInterface.h"

void InitEngine()
{
	MASS::InitEngine();
}

int LoadTextureResource(const char* filename)
{
	return MASS::LoadTextureResource(filename);
}

int LoadSoundResource(const char* filename)
{
	return MASS::LoadSoundResource(filename);
}

void FreeTextureResource(int id)
{
	MASS::FreeTextureResource(id);
}

void FreeSoundResource(int sound)
{
	MASS::FreeSoundResource(sound);
}

void PlayAction_Finish()
{
	MASS::PlayAction(new MASSAction::Finish);
}

void PlayAction_Voiceover(const wchar_t* s)
{
	MASS::PlayAction(new MASSAction::Voiceover(s));
}

void PlayAction_Say_ByName(const wchar_t* n, const wchar_t* c)
{
	MASS::PlayAction(new MASSAction::Say(n, c));
}

void PlayAction_Say_ByActor(int a, const wchar_t* c)
{
	MASS::PlayAction(new MASSAction::Say(a, c));
}

void PlayAction_UseScene(int texture)
{
	MASS::PlayAction(new MASSAction::UseScene(texture));
}

void PlayAction_Enter(int a, char pos)
{
	MASS::PlayAction(new MASSAction::Enter(a, pos));
}

void PlayAction_Exit_ByActor(int a)
{
	MASS::PlayAction(new MASSAction::Exit(a));
}

void PlayAction_Exit_ByPosition(char pos)
{
	MASS::PlayAction(new MASSAction::Exit(pos));
}

void PlayAction_Attack_ByActor(int a)
{
	MASS::PlayAction(new MASSAction::Attack(a));
}

void PlayAction_Attack_ByPosition(char pos)
{
	MASS::PlayAction(new MASSAction::Attack(pos));
}

void PlayAction_Delay(int value)
{
	MASS::PlayAction(new MASSAction::Delay(value));
}

void PlayAction_Retreat_ByActor(int a)
{
	MASS::PlayAction(new MASSAction::Retreat(a));
}

void PlayAction_Retreat_ByPosition(char pos)
{
	MASS::PlayAction(new MASSAction::Retreat(pos));
}

void PlayAction_PlaySound(int sound, int trackIndex, int isLoop)
{
	MASS::PlayAction(new MASSAction::PlaySound(sound, trackIndex, isLoop));
}

void PlayAction_StopSound(int trackIndex)
{
	MASS::PlayAction(new MASSAction::StopSound(trackIndex));
}

int GenerateActor(const wchar_t * name)
{
	return MASS::GenerateActor(name);
}

void AddTextureForActor(int actor, const wchar_t * key, int texture)
{
	MASS::AddTextureForActor(actor, key, texture);
}

void SetActiveTextureForActor(int actor, const wchar_t * key)
{
	MASS::SetActiveTextureForActor(actor, key);
}

#ifdef __cplusplus
}
#endif
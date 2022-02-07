#pragma once
#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) void InitEngine();
	__declspec(dllexport) int LoadTextureResource(const char* filename);
	__declspec(dllexport) int LoadSoundResource(const char* filename);
	__declspec(dllexport) void FreeTextureResource(int id);
	__declspec(dllexport) void FreeSoundResource(int sound);
	__declspec(dllexport) void PlayAction_Finish();
	__declspec(dllexport) void PlayAction_Voiceover(const wchar_t* s);
	__declspec(dllexport) void PlayAction_Say_ByName(const wchar_t* n, const wchar_t* c);
	__declspec(dllexport) void PlayAction_Say_ByActor(int a, const wchar_t* c);
	__declspec(dllexport) void PlayAction_UseScene(int texture);
	__declspec(dllexport) void PlayAction_Enter(int a, char pos);
	__declspec(dllexport) void PlayAction_Exit_ByActor(int a);
	__declspec(dllexport) void PlayAction_Exit_ByPosition(char pos);
	__declspec(dllexport) void PlayAction_Attack_ByActor(int a);
	__declspec(dllexport) void PlayAction_Attack_ByPosition(char pos);
	__declspec(dllexport) void PlayAction_Delay(int value);
	__declspec(dllexport) void PlayAction_Retreat_ByActor(int a);
	__declspec(dllexport) void PlayAction_Retreat_ByPosition(char pos);
	__declspec(dllexport) void PlayAction_PlaySound(int sound, int trackIndex, int isLoop = 0);
	__declspec(dllexport) void PlayAction_StopSound(int trackIndex);
	__declspec(dllexport) int GenerateActor(const wchar_t * name);
	__declspec(dllexport) void AddTextureForActor(int actor, const wchar_t * key, int texture);
	__declspec(dllexport) void SetActiveTextureForActor(int actor, const wchar_t * key);
#ifdef __cplusplus
}
#endif

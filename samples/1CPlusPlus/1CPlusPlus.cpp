#pragma warning(disable:4996)

#include <CInterface.h>

int main(int argc, char* argv[]) {
	InitEngine();
	int t1 = LoadTextureResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\background.jpeg");
	int t2 = LoadTextureResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\Actor1.png");
	int t3 = LoadTextureResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\Actor2.png");
	int s1 = LoadSoundResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\bkg.mp3");
	int s2 = LoadSoundResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\chop.mp3");

	int a1 = GenerateActor(L"张三");
	int a2 = GenerateActor(L"李四");
	AddTextureForActor(a1, L"normal", t2);
	AddTextureForActor(a2, L"normal", t3);
	SetActiveTextureForActor(a1, L"normal");
	SetActiveTextureForActor(a2, L"normal");

	PlayAction_UseScene(t1);
	PlayAction_PlaySound(s1, 1, 1);
	PlayAction_Voiceover(L"这是旁白这是旁白这是旁白");
	PlayAction_Enter(a1, 'L');
	PlayAction_Enter(a2, 'R');
	PlayAction_Say_ByName(L"张三", L"张三说话了张三说话了张三说话了");
	PlayAction_Say_ByActor(a2, L"李四说话了李四说话了李四说话了");
	PlayAction_PlaySound(s2, 2, 0);
	PlayAction_Attack_ByActor(a1);
	PlayAction_Delay(2000);
	PlayAction_PlaySound(s2, 2, 0);
	PlayAction_Attack_ByPosition('R');
	PlayAction_Retreat_ByActor(a1);
	PlayAction_Retreat_ByPosition('R');
	PlayAction_StopSound(1);

	PlayAction_Voiceover(L"这是旁白这是旁白这是旁白");
	PlayAction_Enter(a1, 'L');
	PlayAction_Enter(a2, 'R');
	PlayAction_Say_ByName(L"张三", L"张三说话了张三说话了张三说话了");
	PlayAction_Exit_ByActor(a1);
	PlayAction_Say_ByActor(a2, L"李四说话了李四说话了李四说话了");
	PlayAction_Exit_ByPosition('R');
	PlayAction_Finish();

	FreeTextureResource(t1);
	FreeTextureResource(t2);
	FreeTextureResource(t3);
	FreeSoundResource(s1);
	FreeSoundResource(s2);


	return 0;
}

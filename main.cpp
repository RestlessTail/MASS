#include "CPPInterface.h"
#include "Action.h"
#include "main.h"

#pragma warning(disable:4996)



int main(int argc, char* argv[]) {
	MAS::InitEngine();
	unsigned int r = MAS::LoadTextureResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\background.jpeg");
	unsigned int r2 = MAS::LoadTextureResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\Actor1.png");
	unsigned int r3 = MAS::LoadTextureResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\Actor2.png");
	MAS::Actor_t actor(L"张三", r2);
	MAS::Actor_t actor2(L"李四", r3);
	MAS::SoundSource_t* sound = MAS::LoadSoundResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\bkg.mp3");
	MAS::SoundSource_t* sound2 = MAS::LoadSoundResource("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\chop.mp3");

	MAS::PlayAction(new MASSAction::UseScene(r));
	MAS::PlayAction(new MASSAction::PlaySound(sound, 1));
	MAS::PlayAction(new MASSAction::Voiceover(L"张三和李四出现了"));
	MAS::PlayAction(new MASSAction::Enter(actor, L'L'));
	MAS::PlayAction(new MASSAction::Enter(actor2, L'R'));
	MAS::PlayAction(new MASSAction::Say(L"张三", L"你好吗你好吗你好吗你好吗你好吗你好吗你好吗你好吗你好吗"));
	MAS::PlayAction(new MASSAction::Say(L"李四", L"我很好我很好我很好我很好我很好我很好我很好我很好我很好我很好我很好我很好我很好我很好我很好我很好"));
	MAS::PlayAction(new MASSAction::StopSound(1));
	MAS::PlayAction(new MASSAction::PlaySound(sound2, 2));
	MAS::PlayAction(new MASSAction::Attack(actor));
	MAS::PlayAction(new MASSAction::Delay(5000));
	MAS::PlayAction(new MASSAction::Retreat(actor2));
	MAS::PlayAction(new MASSAction::Finish);
	MAS::PlayAction(new MASSAction::UseScene(r));
	MAS::PlayAction(new MASSAction::Voiceover(L"张三和李四又出现了"));
	
	MAS::FreeTextureResource(r3);
	MAS::FreeSoundResource(sound);
	return 0;
}

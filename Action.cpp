#include "Action.h"
#include "MyArknightsScriptScript.h"
#include "LogView.h"
#include "Mask.h"
#include "Attack.h"
#include "Retreat.h"
#include "Sound.h"
#include "CharacterController.h"

extern irrklang::ISoundEngine* SoundEngine;

MASSAction::Finish::Finish()
{
}

void MASSAction::Finish::exec()
{
    MyArknightsScriptScript::lineControl->clear();
    MyArknightsScriptScript::nameControl->clear();
    //虽然在更新了Scene后其透明度的初始值也是0.0f，但在调用paint()后已经增加了一个步长，因此并不会在这里被删掉
    if (MyArknightsScriptScript::ActiveCharacterL) {
        MyArknightsScriptScript::ActiveCharacterL->unloading = true;
    }
    if (MyArknightsScriptScript::ActiveCharacterR) {
        MyArknightsScriptScript::ActiveCharacterR->unloading = true;
    }
    if (MyArknightsScriptScript::ActiveScene == nullptr || MyArknightsScriptScript::ActiveScene->transparency <= 0.0f) {
        //delete MyArknightsScriptScript::ActiveScene;
        MyArknightsScriptScript::running = false;
    }
    else if (MyArknightsScriptScript::ActiveScene->unloading == false) {
        MyArknightsScriptScript::ActiveScene->loading = false;
        MyArknightsScriptScript::ActiveScene->unloading = true;
    }
}

MASSAction::Voiceover::Voiceover(std::wstring& s)
{
    content = s;
    for (auto& i : content) {
        addToCharacterLibrary(i);
    }
}

MASSAction::Voiceover::Voiceover(const wchar_t* s)
{
    content = s;
    for (auto& i : content) {
        addToCharacterLibrary(i);
    }
}

void MASSAction::Voiceover::exec()
{
    if (!MyArknightsScriptScript::ActionInitFlag) {
        MyArknightsScriptScript::lineControl->clear();
        MyArknightsScriptScript::nameControl->clear();
        /*if (MyArknightsScriptScript::FinishActionNow) { //若收到快进指令，则将两个字符显示的间隔设成0
            lineControl->addCharacterDelay = 0;
        }*/

        MyArknightsScriptScript::lineControl->buffer = content;
        MyArknightsScriptScript::logView->addLog(L"", content);
        MyArknightsScriptScript::ActionInitFlag = true;
    }

    if (MyArknightsScriptScript::lineControl->curCharacter + 1 >= MyArknightsScriptScript::lineControl->buffer.size()) { //执行下一条命令的条件是buffer中的字符已经全部显示    
        MyArknightsScriptScript::WaitForNextAction = true;
    }
}

MASSAction::Say::Say(std::wstring& n, std::wstring& c)
{
    name = n;
    content = c;
    for (auto& i : name) {
        addToCharacterLibrary(i);
    }
    for (auto& i : content) {
        addToCharacterLibrary(i);
    }
}

MASSAction::Say::Say(const wchar_t* n, const wchar_t* c)
{
    name = n;
    content = c;
    for (auto& i : name) {
        addToCharacterLibrary(i);
    }
    for (auto& i : content) {
        addToCharacterLibrary(i);
    }
}

MASSAction::Say::Say(int actor, std::wstring& c)
{
    auto itr = MyArknightsScriptScript::AllActor.find(actor);
    if (itr == MyArknightsScriptScript::AllActor.end()) {
        return;
    }
    name = itr->second->name;
    content = c;
    for (auto& i : name) {
        addToCharacterLibrary(i);
    }
    for (auto& i : content) {
        addToCharacterLibrary(i);
    }
}

MASSAction::Say::Say(int actor, const wchar_t* c)
{
    auto itr = MyArknightsScriptScript::AllActor.find(actor);
    if (itr == MyArknightsScriptScript::AllActor.end()) {
        return;
    }
    name = itr->second->name;
    content = c;
    for (auto& i : name) {
        addToCharacterLibrary(i);
    }
    for (auto& i : content) {
        addToCharacterLibrary(i);
    }
}

void MASSAction::Say::exec()
{
    if (!MyArknightsScriptScript::ActionInitFlag) {
        MyArknightsScriptScript::lineControl->clear();
        MyArknightsScriptScript::nameControl->clear();

        MyArknightsScriptScript::nameControl->buffer = name;
        MyArknightsScriptScript::lineControl->buffer = content;
        MyArknightsScriptScript::logView->addLog(name, content);
        MyArknightsScriptScript::ActionInitFlag = true;
        MyArknightsScriptScript::lineControl->resetCharacterDelay();
    }

    if (MyArknightsScriptScript::ActiveCharacterL) {
        if (MyArknightsScriptScript::ActiveCharacterL->name == MyArknightsScriptScript::nameControl->buffer) {
            MyArknightsScriptScript::ActiveCharacterL->brightness = 1.0f;
        }
        else {
            MyArknightsScriptScript::ActiveCharacterL->brightness = 0.5f;
        }
    }
    if (MyArknightsScriptScript::ActiveCharacterR) {
        if (MyArknightsScriptScript::ActiveCharacterR->name == MyArknightsScriptScript::nameControl->buffer) {
            MyArknightsScriptScript::ActiveCharacterR->brightness = 1.0f;
        }
        else {
            MyArknightsScriptScript::ActiveCharacterR->brightness = 0.5f;
        }
    }

    if (
        MyArknightsScriptScript::lineControl->curCharacter + 1 >= MyArknightsScriptScript::lineControl->buffer.size() ||
        MyArknightsScriptScript::nameControl->curCharacter + 1 >= MyArknightsScriptScript::nameControl->buffer.size()
        ) { //执行下一条命令的条件是buffer中的字符已经全部显示

        MyArknightsScriptScript::WaitForNextAction = true;
    }
}

MASSAction::UseScene::UseScene(int texture)
{
    textureID = texture;
}

void MASSAction::UseScene::exec()
{
    //虽然在更新了Scene后其透明度的初始值也是0.0f，但在调用paint()后已经增加了一个步长，因此并不会在这里被删掉
    if (
        MyArknightsScriptScript::ActiveScene == nullptr ||
        MyArknightsScriptScript::ActiveScene->transparency <= 0.0f
        ) {
        if (MyArknightsScriptScript::ActiveScene) {
            delete MyArknightsScriptScript::ActiveScene;
        }
        MyArknightsScriptScript::ActiveScene = new Scene(textureID);
        MyArknightsScriptScript::nextAction();
        goto end;
    }
    if (MyArknightsScriptScript::ActiveScene->unloading == false) {
        MyArknightsScriptScript::ActiveScene->loading = false;
        MyArknightsScriptScript::ActiveScene->unloading = true;
        
    }
    return;
end:;
    MyArknightsScriptScript::running = false;
}

MASSAction::Enter::Enter(int a, char pos)
{
    auto itr = MyArknightsScriptScript::AllActor.find(a);
    if (itr == MyArknightsScriptScript::AllActor.end()) {
        return;
    }
    actor = itr->second;
    if (pos == 'R') {
        position = 'R';
    }
    else {
        position = 'L';
    }
}

void MASSAction::Enter::exec()
{
    if (position == 'L') {
        //将unloading不断设置成true，直到透明度小于等于0。在此之前不会执行下一道命令。
        if (
            MyArknightsScriptScript::ActiveCharacterL == nullptr ||
            MyArknightsScriptScript::ActiveCharacterL->transparency <= 0.0f
            ) {
            delete MyArknightsScriptScript::ActiveCharacterL;
            //unsigned int textureID = actor.texture;
            MyArknightsScriptScript::ActiveCharacterL = new Actor(actor->name, actor->activeTexture);
            MyArknightsScriptScript::ActiveCharacterL->setupVertex({ -1.2f, 0.7f, 0.0f }, { 0.3f, 0.7f, 0.0f }, { -1.2f, -1.5f, 0.0f }, { 0.3f, -1.5f, 0.0f });
            MyArknightsScriptScript::ActiveCharacterL->reflect = true;
            MyArknightsScriptScript::ActiveCharacterL->name = actor->name;

            //确保这里的操作只进行一次
            MyArknightsScriptScript::nextAction();
        }
        else if (MyArknightsScriptScript::ActiveCharacterL->unloading == false) {
            MyArknightsScriptScript::ActiveCharacterL->loading = false;
            MyArknightsScriptScript::ActiveCharacterL->unloading = true;
        }
    }
    else if (position == 'R') {
        if (
            MyArknightsScriptScript::ActiveCharacterR == nullptr ||
            MyArknightsScriptScript::ActiveCharacterR->transparency <= 0.0f
            ) {
            delete MyArknightsScriptScript::ActiveCharacterR;
            MyArknightsScriptScript::ActiveCharacterR = new Actor(actor->name, actor->activeTexture);
            MyArknightsScriptScript::ActiveCharacterR->setupVertex({ -0.3f, 0.7f, 0.0f }, { 1.2f, 0.7f, 0.0f }, { -0.3f, -1.5f, 0.0f }, { 1.2f, -1.5f, 0.0f });
            MyArknightsScriptScript::ActiveCharacterR->name = actor->name;

            MyArknightsScriptScript::nextAction();
        }
        else if (MyArknightsScriptScript::ActiveCharacterR->unloading == false) {
            MyArknightsScriptScript::ActiveCharacterR->loading = false;
            MyArknightsScriptScript::ActiveCharacterR->unloading = true;
        }
    }
}

MASSAction::Exit::Exit(char pos)
{
    position = pos;
}

MASSAction::Exit::Exit(int actor)
{
    auto itr = MyArknightsScriptScript::AllActor.find(actor);
    if (itr == MyArknightsScriptScript::AllActor.end()) {
        return;
    }
    if (MyArknightsScriptScript::ActiveCharacterL->name == itr->second->name) {
        position = 'L';
    }
    else if (MyArknightsScriptScript::ActiveCharacterR->name == itr->second->name) {
        position = 'R';
    }
}

void MASSAction::Exit::exec()
{
    if (position == 'L') {
        MyArknightsScriptScript::ActiveCharacterL->unloading = true;
    }
    else if (position == 'R') {
        MyArknightsScriptScript::ActiveCharacterR->unloading = true;
    }

    MyArknightsScriptScript::nextAction();
}

MASSAction::Attack::Attack(int a)
{
    auto itr = MyArknightsScriptScript::AllActor.find(a);
    if (itr == MyArknightsScriptScript::AllActor.end()) {
        return;
    }
    if (MyArknightsScriptScript::ActiveCharacterL->name == itr->second->name) {
        position = 'L';
    }
    else if (MyArknightsScriptScript::ActiveCharacterR->name == itr->second->name) {
        position = 'R';
    }
}

MASSAction::Attack::Attack(char pos)
{
    position = pos;
}

void MASSAction::Attack::exec()
{
    if (!MyArknightsScriptScript::ActionInitFlag) {

        MyArknightsScriptScript::mask->visible = true;
        if (position == 'L') {
            MyArknightsScriptScript::attack->enable('L');
        }
        else if (position == 'R') {
            MyArknightsScriptScript::attack->enable('R');
        }

        MyArknightsScriptScript::ActionInitFlag = true;
    }

    if (MyArknightsScriptScript::attack->finished) {
        MyArknightsScriptScript::mask->visible = false;
        MyArknightsScriptScript::attack->disable();
        MyArknightsScriptScript::nextAction();
    }
}

MASSAction::Delay::Delay(int value)
{
    val = value;
}

void MASSAction::Delay::exec()
{
    if (!MyArknightsScriptScript::ActionInitFlag) {
        MyArknightsScriptScript::delayFrame = static_cast<int>(0.06f * val);

        MyArknightsScriptScript::ActionInitFlag = true;
    }
    if (MyArknightsScriptScript::delayFrame <= 0) {
        MyArknightsScriptScript::nextAction();
    }
    else {
        --MyArknightsScriptScript::delayFrame;
    }
}

MASSAction::Retreat::Retreat(int a)
{
    auto itr = MyArknightsScriptScript::AllActor.find(a);
    if (itr == MyArknightsScriptScript::AllActor.end()) {
        return;
    }
    if (MyArknightsScriptScript::ActiveCharacterL->name == itr->second->name) {
        position = 'L';
    }
    else if (MyArknightsScriptScript::ActiveCharacterR->name == itr->second->name) {
        position = 'R';
    }
}

MASSAction::Retreat::Retreat(char pos)
{
    position = pos;
}

void MASSAction::Retreat::exec()
{
    if (!MyArknightsScriptScript::ActionInitFlag) {
        if (position == 'L') {
            MyArknightsScriptScript::retreat->enable('L');
        }
        else if (position == 'R'){
            MyArknightsScriptScript::retreat->enable('R');
        }

        MyArknightsScriptScript::ActionInitFlag = true;
    }

    if (MyArknightsScriptScript::retreat->finished) {
        MyArknightsScriptScript::retreat->disable();
        MyArknightsScriptScript::nextAction();
        goto end;
    }
end:;
}

MASSAction::PlaySound::PlaySound(int soundID, int trackIndex, bool isLoop)
{
    soundSource = soundID;
    track = trackIndex;
    loop = isLoop;
}

void MASSAction::PlaySound::exec()
{
    auto itr = MyArknightsScriptScript::AllSoundSource.find(soundSource);
    if (itr == MyArknightsScriptScript::AllSoundSource.end()) {
        return;
    }
    MASSSoundSource* s = new MASSSoundSource;
    s->irrSoundSource = itr->second;
    s->loop = loop;
    MyArknightsScriptScript::soundEngine->AddToTrack(s, track);

    MyArknightsScriptScript::nextAction();
}

MASSAction::StopSound::StopSound(int trackIndex)
{
    track = trackIndex;
}

void MASSAction::StopSound::exec()
{
    MyArknightsScriptScript::soundEngine->StopTrack(track);
    MyArknightsScriptScript::nextAction();
}
#include "Action.h"
#include "MyArknightsScriptScript.h"
#include "LogView.h"
#include "Symbol.h"
#include "Mask.h"
#include "Attack.h"
#include "Retreat.h"

extern ActorTableType ActorTable;
extern SceneTableType SceneTable;

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
        delete MyArknightsScriptScript::ActiveScene;
        MyArknightsScriptScript::running = false;
    }
    else if (MyArknightsScriptScript::ActiveScene->unloading == false) {
        MyArknightsScriptScript::ActiveScene->loading = false;
        MyArknightsScriptScript::ActiveScene->unloading = true;
    }
}

void MASSAction::Finish::Export(std::wstring& str)
{
    return;
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

void MASSAction::Voiceover::Export(std::wstring& str)
{
    str += (
        L"Voiceover \"" + content + L"\"\n"
        );
}

void MASSAction::Say::exec()
{
    if (!MyArknightsScriptScript::ActionInitFlag) {
        MyArknightsScriptScript::lineControl->clear();
        MyArknightsScriptScript::nameControl->clear();

        std::wstring name = ActorTable.find(key)->second.Name;

        MyArknightsScriptScript::nameControl->buffer = name;
        MyArknightsScriptScript::lineControl->buffer = content;
        MyArknightsScriptScript::logView->addLog(name, content);
        MyArknightsScriptScript::ActionInitFlag = true;
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

void MASSAction::Say::Export(std::wstring& str)
{
    str += (
        key + L" Say \"" + content + L"\"\n"
        );
}

void MASSAction::UseScene::exec()
{
    //虽然在更新了Scene后其透明度的初始值也是0.0f，但在调用paint()后已经增加了一个步长，因此并不会在这里被删掉
    if (
        MyArknightsScriptScript::ActiveScene == nullptr ||
        MyArknightsScriptScript::ActiveScene->transparency <= 0.0f
        ) {
        delete MyArknightsScriptScript::ActiveScene;
        unsigned int textureID = SceneTable.find(key)->second.id;
        MyArknightsScriptScript::ActiveScene = new Scene(textureID);
        MyArknightsScriptScript::nextAction();
        goto end;
    }
    if (MyArknightsScriptScript::ActiveScene->unloading == false) {
        MyArknightsScriptScript::ActiveScene->loading = false;
        MyArknightsScriptScript::ActiveScene->unloading = true;
    }
end:;
}

void MASSAction::UseScene::Export(std::wstring& str)
{
    str += (
        L"UseScene " + key + L"\n"
        );
}

void MASSAction::Enter::exec()
{
    if (position == L'L') {
        //将unloading不断设置成true，直到透明度小于等于0。在此之前不会执行下一道命令。
        if (
            MyArknightsScriptScript::ActiveCharacterL == nullptr ||
            MyArknightsScriptScript::ActiveCharacterL->transparency <= 0.0f
            ) {
            delete MyArknightsScriptScript::ActiveCharacterL;
            auto data = ActorTable.find(key);
            unsigned int textureID = data->second.Texture;
            MyArknightsScriptScript::ActiveCharacterL = new Actor(textureID);
            MyArknightsScriptScript::ActiveCharacterL->setupVertex({ -1.2f, 0.7f, 0.0f }, { 0.3f, 0.7f, 0.0f }, { -1.2f, -1.5f, 0.0f }, { 0.3f, -1.5f, 0.0f });
            MyArknightsScriptScript::ActiveCharacterL->reflect = true;
            MyArknightsScriptScript::ActiveCharacterL->name = data->second.Name;

            //确保这里的操作只进行一次
            MyArknightsScriptScript::nextAction();
        }
        else if (MyArknightsScriptScript::ActiveCharacterL->unloading == false) {
            MyArknightsScriptScript::ActiveCharacterL->loading = false;
            MyArknightsScriptScript::ActiveCharacterL->unloading = true;
        }
    }
    else {
        if (
            MyArknightsScriptScript::ActiveCharacterR == nullptr ||
            MyArknightsScriptScript::ActiveCharacterR->transparency <= 0.0f
            ) {
            delete MyArknightsScriptScript::ActiveCharacterR;
            auto data = ActorTable.find(key);
            unsigned int textureID = data->second.Texture;
            MyArknightsScriptScript::ActiveCharacterR = new Actor(textureID);
            MyArknightsScriptScript::ActiveCharacterR->setupVertex({ -0.3f, 0.7f, 0.0f }, { 1.2f, 0.7f, 0.0f }, { -0.3f, -1.5f, 0.0f }, { 1.2f, -1.5f, 0.0f });
            MyArknightsScriptScript::ActiveCharacterR->name = data->second.Name;

            MyArknightsScriptScript::nextAction();
        }
        else if (MyArknightsScriptScript::ActiveCharacterR->unloading == false) {
            MyArknightsScriptScript::ActiveCharacterR->loading = false;
            MyArknightsScriptScript::ActiveCharacterR->unloading = true;
        }
    }
}

void MASSAction::Enter::Export(std::wstring& str)
{
    str += (
        L"Enter " + key + L" " + position + L"\n"
        );
}

void MASSAction::Exit::exec()
{
    if (flag == L'K') {
        if (id == L"L") {
            MyArknightsScriptScript::ActiveCharacterL->unloading = true;
        }
        else {
            MyArknightsScriptScript::ActiveCharacterR->unloading = true;
        }
    }
    else {
        auto data = ActorTable.find(id);
        std::wstring name = data->second.Name;

        if (MyArknightsScriptScript::ActiveCharacterL->name == name) {
            MyArknightsScriptScript::ActiveCharacterL->unloading = true;
        }
        else if (MyArknightsScriptScript::ActiveCharacterR->name == name) {
            MyArknightsScriptScript::ActiveCharacterR->unloading = true;
        }
        /*else {
            std::wstring msg = L"No such character named " + name + L". No character will exit.";
            RTWarning(msg.c_str());
        }*/
    }
    MyArknightsScriptScript::nextAction();
}

void MASSAction::Exit::Export(std::wstring& str)
{
    str += (
        L"Exit " + id + L"\n"
        );
}

void MASSAction::Attack::exec()
{
    if (!MyArknightsScriptScript::ActionInitFlag) {
        std::wstring name = ActorTable.find(id)->second.Name;

        MyArknightsScriptScript::mask->visible = true;
        if (MyArknightsScriptScript::ActiveCharacterL->name == name) {
            MyArknightsScriptScript::attack->enable('L');
        }
        else {
            MyArknightsScriptScript::attack->enable('R');
        }

        MyArknightsScriptScript::ActionInitFlag = true;
    }

    if (MyArknightsScriptScript::attack->finished) {
        MyArknightsScriptScript::mask->visible = false;
        MyArknightsScriptScript::attack->disable();
        MyArknightsScriptScript::nextAction();
        //goto end;
    }
//end:;
}

void MASSAction::Attack::Export(std::wstring& str)
{
    str += (
        id + L" Attack\n"
        );
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

void MASSAction::Delay::Export(std::wstring& str)
{
    str += (
        L"Delay " + std::to_wstring(val) + L"\n"
        );
}

void MASSAction::Retreat::exec()
{
    if (!MyArknightsScriptScript::ActionInitFlag) {
        std::wstring name = ActorTable.find(id)->second.Name;

        if (MyArknightsScriptScript::ActiveCharacterL->name == name) {
            MyArknightsScriptScript::retreat->enable('L');
            //ActiveCharacterL->unloading = true;
        }
        else {
            MyArknightsScriptScript::retreat->enable('R');
            //ActiveCharacterL->unloading = true;
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

void MASSAction::Retreat::Export(std::wstring& str)
{
    str += (
        id + L" Retreat\n"
        );
}

#include "LogView.h"
#include "MyArknightsScriptScript.h"
#include <malloc.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CharacterController.h"

#pragma warning(disable:4996)

//每34个字换一次行
constexpr int Wrap = 35;//（加\0）
constexpr int wrap = 34;//（不加\0）

extern int characterHeight;

float LogView::cursor = 0.0f;
bool LogView::allowScroll = false;

LogView::LogView()
{
    scale = 1.0f;
    visible = false;
}

void LogView::paint()
{
    //绘制文字
    MyArknightsScriptScript::loader->SetActiveProgram(1);
    //MyArknightsScriptScript::loader->setUniformData("projection", projection);

    //先计算要显示的第一行
    int curLine = static_cast<int>(cursor);
    float advance = cursor - curLine;
    int rowSpace = characterHeight;

    int winW, winH;
    glfwGetWindowSize(MyArknightsScriptScript::MainWindow, &winW, &winH);

    //计算第一行的坐标
    //将X轴方向上的起始点用一个变量保存起来，以便统一修改。重新确定X轴起始点的坐标
    float contentOriX = -0.35f;
    float nameOriX = -0.41f;
    float curX = contentOriX;
    float curY = 1 - (characterHeight * scale + rowSpace) / winH * (1 - advance);
    
    //对于接下去的每一行
    while (curLine < allLogs.size() && curY >= (-1 - characterHeight * scale / winH)) {
        wchar_t* content = allLogs[curLine].content;
        size_t len = wcslen(content);
        MyArknightsScriptScript::loader->setUniformData("textColor", 1.0f, 1.0f, 1.0f);
        scale = 1.0f;
        for (size_t i = 0; i < len; ++i) {
            wchar_t c = content[i];

            Character* cStruct = getCharacterStruct(c);
            LineCharacter ch;

            float w = static_cast<float>(cStruct->size.x);
            float h = static_cast<float>(cStruct->size.y);

            ch.setupVertex(
                { curX + 1.0f * cStruct->bearing.x * 0.0008f * scale, curY + (1.0f * cStruct->bearing.y - h) * 0.00143f * scale, 0.0f },
                { curX + (1.0f * cStruct->bearing.x + w) * 0.0008f * scale, curY + (1.0f * cStruct->bearing.y - h) * 0.00143f * scale, 0.0f },
                { curX + 1.0f * cStruct->bearing.x * 0.0008f * scale, curY + 1.0f * cStruct->bearing.y * 0.00143f  * scale, 0.0f },
                { curX + (1.0f * cStruct->bearing.x + w) * 0.0008f * scale, curY + 1.0f * cStruct->bearing.y * 0.00143f * scale, 0.0f }
            );
            ch.useTexture(cStruct->textureID);

            ch.paint();
            curX += (static_cast<float>(cStruct->advance >> 6)) * 0.0008f * scale;
        }

        curX = nameOriX;

        wchar_t* name = allLogs[curLine].name;
        if (name) {
            MyArknightsScriptScript::loader->setUniformData("textColor", 0.5f, 0.5f, 0.5f);
            scale = 1.1f;
            len = wcslen(name);
            for (size_t i = 1; i <= len; ++i) {
                wchar_t c = name[len - i];

                Character* cStruct = getCharacterStruct(c);
                LineCharacter ch;

                float w = static_cast<float>(cStruct->size.x);
                float h = static_cast<float>(cStruct->size.y);

                ch.setupVertex(
                    { curX + (1.0f * cStruct->bearing.x - w) * 0.0008f * scale, curY + (1.0f * cStruct->bearing.y - h) * 0.00143f * scale, 0.0f },
                    { curX + 1.0f * cStruct->bearing.x * 0.0008f * scale, curY + (1.0f * cStruct->bearing.y - h) * 0.00143f * scale, 0.0f },
                    { curX + (1.0f * cStruct->bearing.x - w) * 0.0008f * scale, curY + 1.0f * cStruct->bearing.y * 0.00143f * scale, 0.0f },
                    { curX + 1.0f * cStruct->bearing.x * 0.0008f * scale, curY + 1.0f * cStruct->bearing.y * 0.00143f * scale, 0.0f }
                );
                ch.useTexture(cStruct->textureID);

                ch.paint();
                curX -= (static_cast<float>(cStruct->advance >> 6)) * 0.0008f * scale;
            }
            curX = nameOriX;
        }
        ++curLine;
        curX = contentOriX;
        curY -= (characterHeight * scale + rowSpace) * 0.00143f * scale;
    }
}

void LogView::addLog(std::wstring name, std::wstring& content)
{
    //为了方便显示，这个函数会把一个长的句子拆成若干个短句
    size_t nameSize = name.size();
    size_t contentSize = content.size();

    wchar_t* nameStr = nullptr;
    wchar_t* contentStr = nullptr;

    if (nameSize != 0) {
        nameStr = (wchar_t*)calloc(nameSize + 1, sizeof(wchar_t));
        if (nameStr) {
            wcscpy(nameStr, name.c_str());
        }
    }

    contentStr = (wchar_t*)calloc(Wrap, sizeof(wchar_t));
    const wchar_t* contentStrBegin = content.c_str();
    if (contentStr) {
        wcsncpy(contentStr, contentStrBegin, wrap);
    }

    allLogs.push_back({ nameStr, contentStr });

    if (contentSize > 34) {
        size_t begin = 34;
        while (begin < contentSize) {
            wchar_t* cp = (wchar_t*)calloc(Wrap, sizeof(wchar_t));
            if (cp) {
                wcscpy(cp, contentStrBegin + begin);
                begin += wcslen(cp);
            }
            allLogs.push_back({ nullptr, cp });
        }
    }
}

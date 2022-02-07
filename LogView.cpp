#include "LogView.h"
#include "MyArknightsScriptScript.h"
#include <malloc.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CharacterController.h"

#pragma warning(disable:4996)

//ÿ34���ֻ�һ����
constexpr int Wrap = 35;//����\0��
constexpr int wrap = 34;//������\0��

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
    //��������
    MyArknightsScriptScript::loader->SetActiveProgram(1);
    //MyArknightsScriptScript::loader->setUniformData("projection", projection);

    //�ȼ���Ҫ��ʾ�ĵ�һ��
    int curLine = static_cast<int>(cursor);
    float advance = cursor - curLine;
    int rowSpace = characterHeight;

    int winW, winH;
    glfwGetWindowSize(MyArknightsScriptScript::MainWindow, &winW, &winH);

    //�����һ�е�����
    //��X�᷽���ϵ���ʼ����һ�����������������Ա�ͳһ�޸ġ�����ȷ��X����ʼ�������
    float contentOriX = -0.35f;
    float nameOriX = -0.41f;
    float curX = contentOriX;
    float curY = 1 - (characterHeight * scale + rowSpace) / winH * (1 - advance);
    
    //���ڽ���ȥ��ÿһ��
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
    //Ϊ�˷�����ʾ������������һ�����ľ��Ӳ�����ɸ��̾�
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

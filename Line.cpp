#include "Line.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MyArknightsScriptScript.h"
#include "CharacterController.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//glm::mat4 LineControl::projection;

extern FT_Face FTFace;
extern int characterHeight;

LineControl::LineControl(float posx, float posy){
    visible = true;
    curCharacter = 0;
    addCharacterDelay = 5;
    curDelay = 0;
    rowSpace = 10;
    linePosX = posx;
    linePosY = posy;
    xPos = 0.0f;
    yPos = 0.0f;
    color = { 1.0f, 1.0f, 1.0f };
    alignment = LineAlignment::LEFT;
    scale = 1.0;
   
    /*projection = glm::ortho(0.0f,
        static_cast<float>(MyArknightsScriptScript::windowWidth),
        0.0f,
        static_cast<float>(MyArknightsScriptScript::windowHeight)
    );*/

}

void LineControl::paint(){
    MyArknightsScriptScript::loader->SetActiveProgram(1);
    //MyArknightsScriptScript::loader->setUniformData("projection", projection);
    MyArknightsScriptScript::loader->setUniformData("textColor", color.r, color.g, color.b);
    if (curDelay <= 0 && curCharacter < buffer.size()) {
        addCharacter();
        curDelay = addCharacterDelay;
    }
    for (auto c : characters) {
        c->paint();
    }
    --curDelay;
    MyArknightsScriptScript::loader->SetActiveProgram(0);
}

void LineControl::clear(){
    for (auto c : characters) {
        delete c;
    }
    characters.clear();
    buffer.clear();
    xPos = 0.0f;
    yPos = 0.0f;
    curCharacter = 0;
}

void LineControl::nextLine(){
    //std::string curLine = (buffer.begin())->content;
    //int line = curLine.size();
}

void LineControl::addCharacter(){
    wchar_t c;
    if (alignment == LineAlignment::LEFT) {
        c = buffer[curCharacter];
    }
    else {
        c = buffer[buffer.size() - curCharacter - 1];
    }

    Character* cStruct = getCharacterStruct(c);
    LineCharacter* ch = new LineCharacter;    

    //xPos += ch->bearing.x;
    updateyPos();

    float w = static_cast<float>(cStruct->size.x);
    float h = static_cast<float>(cStruct->size.y);

    int winW, winH;
    glfwGetWindowSize(MyArknightsScriptScript::MainWindow, &winW, &winH);

    ch->setupVertex(
        { linePosX + (xPos + cStruct->bearing.x) * 0.0008f * scale, linePosY + (yPos + cStruct->bearing.y - h) * 0.00143f * scale, 0.0f },
        { linePosX + (xPos + cStruct->bearing.x + w) * 0.0008f * scale, linePosY + (yPos + cStruct->bearing.y - h) * 0.00143f * scale, 0.0f },
        { linePosX + (xPos + cStruct->bearing.x) * 0.0008f * scale, linePosY + (yPos + cStruct->bearing.y) * 0.00143f * scale, 0.0f },
        { linePosX + (xPos + cStruct->bearing.x + w) * 0.0008f * scale, linePosY + (yPos + cStruct->bearing.y) * 0.00143f * scale, 0.0f }
    );
    ch->useTexture(cStruct->textureID);
    
    if (curCharacter % 30 == 29) {
        xPos = linePosX;
    }
    else {
        if (alignment == LineAlignment::LEFT) {
            xPos += (cStruct->advance >> 6); // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
        }
        else {
            xPos -= (cStruct->advance >> 6); // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
        }
    }

    characters.push_back(ch);
    ++curCharacter;
}

void LineControl::skip()
{
    addCharacterDelay = 0;
}

void LineControl::resetCharacterDelay()
{
    addCharacterDelay = 5;
}

void LineControl::updateyPos(){
    yPos = linePosX - (curCharacter / 30 * (characterHeight * scale + rowSpace) - rowSpace);
}

void LineCharacter::paint(){
    
    applyVertex();
    applyTexture();
    callOpenGL();
}

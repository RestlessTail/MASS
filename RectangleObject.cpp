#include "RectangleObject.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "TextureLoader.h"
#include "MyArknightsScriptScript.h"

RectangleObject::RectangleObject(){
    float v[] = {
        //---- 位置 ---    - 纹理坐标 -
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,    // 左上
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,   // 右上
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // 左下
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f   // 右下
        
    };
    memcpy(vertex, v, sizeof(v));
}

RectangleObject::~RectangleObject()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void RectangleObject::paint()
{
    MyArknightsScriptScript::loader->setUniformData("transparency", 1.0f);
    applyVertex();
    applyTexture();
    callOpenGL();
}

void RectangleObject::setupVertex(VertexCoord topLeft, VertexCoord topRight, VertexCoord bottomLeft, VertexCoord bottomRight){
    memcpy(vertex, &topLeft, sizeof(VertexCoord));
    memcpy(vertex + 5, &topRight, sizeof(VertexCoord));
    memcpy(vertex + 10, &bottomLeft, sizeof(VertexCoord));
    memcpy(vertex + 15, &bottomRight, sizeof(VertexCoord));

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int indices[] = { // 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        0, 2, 3  // 第二个三角形
    };

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void RectangleObject::setupTextures(std::string path){
    TextureLoader::loadTexture(&texture, path);
}

void RectangleObject::useTexture(unsigned int t){
    texture = t;
}

void RectangleObject::setTextureVertex(GlyphRange range){
    memcpy(vertex + 3, &range.topLeft, sizeof(TextureCoord));
    memcpy(vertex + 8, &range.topRight, sizeof(TextureCoord));
    memcpy(vertex + 13, &range.bottomLeft, sizeof(TextureCoord));
    memcpy(vertex + 18, &range.bottomRight, sizeof(TextureCoord));
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
}

void RectangleObject::setVertex(VertexCoord topLeft, VertexCoord topRight, VertexCoord bottomLeft, VertexCoord bottomRight){
    memcpy(vertex, &topLeft, sizeof(VertexCoord));
    memcpy(vertex + 5, &topRight, sizeof(VertexCoord));
    memcpy(vertex + 10, &bottomLeft, sizeof(VertexCoord));
    memcpy(vertex + 15, &bottomRight, sizeof(VertexCoord));
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
}

void RectangleObject::applyTexture(){
    TextureLoader::applyTexture(texture);
}

void RectangleObject::applyVertex(){
    glBindVertexArray(VAO);
}

void RectangleObject::callOpenGL(){
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

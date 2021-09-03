#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Character {
	unsigned int textureID;
	glm::ivec2 size;       // 字形大小
	glm::ivec2 bearing;    // 从基准线到字形左部/顶部的偏移值
	unsigned int advance;    // 原点距下一个字形原点的距离
};

void CharacterControllerInit();
void addToCharacterLibrary(wchar_t ch);
Character* getCharacterStruct(wchar_t ch);


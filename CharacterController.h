#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Character {
	unsigned int textureID;
	glm::ivec2 size;       // ���δ�С
	glm::ivec2 bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
	unsigned int advance;    // ԭ�����һ������ԭ��ľ���
};

void CharacterControllerInit();
void addToCharacterLibrary(wchar_t ch);
Character* getCharacterStruct(wchar_t ch);


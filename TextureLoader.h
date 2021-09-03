//TextureLoader.h : 此文件用于从本地加载材质

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H


typedef std::unordered_map<std::string, unsigned int> TextureSetType;

namespace TextureLoader{
	void loadTexture(unsigned int* dest, std::wstring path);
	void loadTexture(unsigned int* dest, std::string path);
	void loadTexture(unsigned int* dest, FT_Face face);
	void applyTexture(unsigned int texture);
};


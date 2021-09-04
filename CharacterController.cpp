#include "CharacterController.h"
#include <iostream>
#include "TextureLoader.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <filesystem>

static FT_Library ft;
FT_Face FTFace;
int characterHeight; //文字高度（宽度自动计算）
static std::unordered_map<wchar_t, Character> characterLibrary;
extern std::filesystem::path curPath;
extern std::filesystem::path scriptPath;

void CharacterControllerInit()
{
    std::filesystem::current_path(curPath);

    characterHeight = 48;

    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    if (FT_New_Face(ft, "font/SourceHanSansCN-Normal.otf", 0, &FTFace)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(FTFace, 0, characterHeight);

    std::filesystem::current_path(scriptPath);
}

void addToCharacterLibrary(wchar_t ch)
{
    Character c;
    if (FT_Load_Char(FTFace, ch, FT_LOAD_RENDER))
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

    unsigned int id;
    TextureLoader::loadTexture(&id, FTFace);

    c.textureID = id;
    c.size = glm::ivec2(FTFace->glyph->bitmap.width, FTFace->glyph->bitmap.rows);
    c.bearing = glm::ivec2(FTFace->glyph->bitmap_left, FTFace->glyph->bitmap_top);
    c.advance = FTFace->glyph->advance.x;

    characterLibrary.emplace(ch, c);
}

Character* getCharacterStruct(wchar_t ch)
{
    auto itr = characterLibrary.find(ch);
    if (itr != characterLibrary.end()) {
        return &(itr->second);
    }
    else {
        return nullptr;
    }
}
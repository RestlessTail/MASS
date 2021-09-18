#pragma once
#include <unordered_map>
#include "interpreter/syntax.h"

typedef struct {
	unsigned int id;
	std::wstring path;
} InMemorySceneData;

typedef struct {
	std::wstring Name;
	unsigned int Texture;
	std::wstring TexturePath;
} InMemoryActorData;

typedef struct {
	std::wstring Filename;
	bool mix;
	bool loop;;
} InMemorySoundData;

typedef std::unordered_map<std::wstring, InMemorySceneData> SceneTableType;
typedef std::unordered_map<std::wstring, InMemoryActorData> ActorTableType;
typedef std::unordered_map<std::wstring, InMemorySoundData> SoundTableType;

namespace Symbol
{
	void LoadSymbols();
};


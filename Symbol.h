#pragma once
#include <unordered_map>
#include "interpreter/syntax.h"

typedef struct {
	std::wstring Name;
	unsigned int Texture;
} InMemoryActorData;

typedef std::unordered_map<std::wstring, unsigned int> SceneTableType;
typedef std::unordered_map<std::wstring, InMemoryActorData> ActorTableType;

namespace Symbol
{
	void LoadSymbols();
};


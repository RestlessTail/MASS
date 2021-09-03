#pragma once

#include <list>
#include <string>
#include <stdio.h>
#include <unordered_map>
#include "lexer.h"

enum class VariableType{
	CHARACTER = 0,
	SCENE
};

struct CharacterDataType{
	std::wstring Name;
	std::wstring Texture;
};

struct SceneDataType{
	std::wstring Texture;
};

enum class ActionType{
	SAY = 0,
	VOICEOVER,
	USESCENE,
	ENTER,
	EXIT,
	FINISH,
	ATTACK,
	DELAY,
	RETREAT
};

typedef std::wstring SymbolTableKey;

struct SymbolTableValue{
	VariableType type;
	void* data;
};

struct ActionTableItem{
	ActionType type;
	std::list<std::wstring> data;
	long int property;
};

typedef std::unordered_map<SymbolTableKey, SymbolTableValue> SymbolTableType;
typedef std::list<ActionTableItem> ActionTableType;

void Parse(FILE* fp);
Token GetNextToken(FILE* fp);
void ParseKeyword(FILE* fp);
void ParseDefineCharacter(FILE* fp);

//返回0：已经完成所有属性的解析
//返回1：未完成解析
int ParseCharacterProperty(CharacterDataType* data, FILE* fp);
void ParseDefineScene(FILE* fp);
int ParseSceneProperty(SceneDataType* data, FILE* fp);
void ParseVoiceover(FILE* fp);
void ParseIdentifier(FILE* fp);
void ParseSay(SymbolTableKey& key, FILE* fp); 
void ParseUseScene(FILE* fp); 
void ParseEnter(FILE* fp);
void ParseExit(FILE* fp);
void ParseAttack(SymbolTableKey& key);
void ParseDelay(FILE* fp);
void ParseRetreat(SymbolTableKey& key);
void SyntaxError(std::wstring msg);
void SyntaxWarning(std::wstring msg);

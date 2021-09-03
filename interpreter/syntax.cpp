#include "lexer.h"
#include "syntax.h"
#include <string>
#include <stdio.h>
#include "../CharacterController.h"

extern wchar_t StringIdentifierValue[ID_LEN_MAX]; //储存标识符的名称
extern wchar_t StringConstantValue[STRING_LEN_MAX]; //储存字符串常量
extern int IntengerConstantValue; //储存整数常量
extern double FloatConstantValue; //储存浮点常量
extern LexerErr ErrCode; //若解析错误，则将错误原因保存在这里
extern Keyword KeywordValue; //保存关键词的枚举值
int MASSErrors;
int MASSWarnings;
extern int curRow;
extern int curCol;

Token CurToken;

SymbolTableType SymbolTable;
ActionTableType ActionTable;

Token GetNextToken(FILE* fp) {
	CurToken = GetToken(fp);
	return CurToken;
}

void Parse(FILE* fp) {
	CharacterControllerInit();
	curRow = 1;
	GetNextToken(fp);
	while(CurToken != Token::TOKEN_EOF) {
		switch(CurToken) {
			case Token::TOKEN_KEYWORD:
				ParseKeyword(fp);
				break;
			case Token::TOKEN_IDENTIFIER:
				ParseIdentifier(fp);
				break;
			default:
				SyntaxError(L"Missing Keyword.");
				GetNextToken(fp);
				break;
		}
		GetNextToken(fp);
	}
	ActionTableItem item;
	item.type = ActionType::FINISH;
	ActionTable.push_back(item);
}

void ParseKeyword(FILE* fp) {
	switch (KeywordValue) {
		case Keyword::DEFINECHARACTER:
			ParseDefineCharacter(fp);
			break;
		case Keyword::DEFINESCENE:
			ParseDefineScene(fp);
			break;
		case Keyword::VOICEOVER:
			ParseVoiceover(fp);
			break;
		case Keyword::USESCENE:
			ParseUseScene(fp);
			break;
		case Keyword::ENTER:
			ParseEnter(fp);
			break;
		case Keyword::EXIT:
			ParseExit(fp);
			break;
		case Keyword::DELAY:
			ParseDelay(fp);
			break;
		default:
			SyntaxError(L"Misused keyword.");
			GetNextToken(fp);
	}
}

void ParseDefineCharacter(FILE* fp) {
	SymbolTableValue value;
	GetNextToken(fp);
	if(CurToken != Token::TOKEN_IDENTIFIER) {
		SyntaxError(L"Expected identifier after \"DefineCharacter\".");
		return;
	}

	SymbolTableKey key = StringIdentifierValue;
	value.type = VariableType::CHARACTER;

	GetNextToken(fp);
	if(CurToken != Token::TOKEN_KEYWORD || KeywordValue != Keyword::WHOSE) {
		SyntaxError(L"Expected keyword \"Whose\" after the identifier.");
		return;
	}

	CharacterDataType* data = new CharacterDataType;
	while(ParseCharacterProperty(data, fp)) {
		//解析属性
	}

	value.data = data;

	SymbolTable.emplace(key, value);
}

//返回0：已经完成所有属性的解析
//返回1：未完成解析
int ParseCharacterProperty(CharacterDataType* data, FILE* fp) {
	GetNextToken(fp);
	if(CurToken != Token::TOKEN_KEYWORD) {
		SyntaxError(L"Missing keyword in the property group.");
	}

	switch(KeywordValue) {
		case Keyword::ENDDEFINE:
			return 0;
		case Keyword::NAME:
			GetNextToken(fp);
			if(CurToken != Token::TOKEN_STRING) {
				SyntaxError(L"Expected string after \"Name\".");
				break;
			}
			data->Name = std::wstring(StringConstantValue);
			break;
		case Keyword::TEXTURE:
			GetNextToken(fp);
			if(CurToken != Token::TOKEN_STRING) {
				SyntaxError(L"Expected string after \"Texture\".");
				break;
			}
			data->Texture = std::wstring(StringConstantValue);
			break;
		default:
			SyntaxError(L"Unexpected keyword in the property group.");
			break;
	}

	return 1;
}

void ParseDefineScene(FILE* fp) {
	SymbolTableValue value;
	GetNextToken(fp);
	if(CurToken != Token::TOKEN_IDENTIFIER) {
		SyntaxError(L"Expected identifier after \"DefineScene\".");
		return;
	}

	SymbolTableKey key = StringIdentifierValue;
	value.type = VariableType::SCENE;

	GetNextToken(fp);
	if(CurToken != Token::TOKEN_KEYWORD || KeywordValue != Keyword::WHOSE) {
		SyntaxError(L"Expected keyword \"Whose\" after the identifier.");
		return;
	}

	SceneDataType* data = new SceneDataType;
	while(ParseSceneProperty(data, fp)) {
		//解析属性
	}

	value.data = data;

	SymbolTable.emplace(key, value);
}

//返回0：已经完成所有属性的解析
//返回1：未完成解析
int ParseSceneProperty(SceneDataType* data, FILE* fp) {
	GetNextToken(fp);
	if(CurToken != Token::TOKEN_KEYWORD) {
		SyntaxError(L"Missing keyword in the property group.");
	}

	switch(KeywordValue) {
		case Keyword::ENDDEFINE:
			return 0;
		case Keyword::TEXTURE:
			GetNextToken(fp);
			if(CurToken != Token::TOKEN_STRING) {
				SyntaxError(L"Expected string after \"Texture\".");
				break;
			}
			data->Texture = std::wstring(StringConstantValue);
			break;
		default:
			SyntaxError(L"Unexpected keyword in the property group.");
			break;
	}

	return 1;
}

void ParseVoiceover(FILE* fp) {
	GetNextToken(fp);
	if(CurToken != Token::TOKEN_STRING) {
		SyntaxError(L"Expected string after \"Voiceover\".");
		return;
	}

	ActionTableItem item;
	item.type = ActionType::VOICEOVER;
	item.data.push_back(std::wstring(StringConstantValue));

	ActionTable.push_back(item);
}

void ParseIdentifier(FILE* fp) {
	SymbolTableKey key = std::wstring(StringIdentifierValue);

	GetNextToken(fp);
	if(CurToken != Token::TOKEN_KEYWORD) {
		SyntaxError(L"Expected keyword after the identifier.");
	}

	switch(KeywordValue) {
		case Keyword::SAY:
			ParseSay(key, fp);
			break;
		case Keyword::ATTACK:
			ParseAttack(key);
			break;
		case Keyword::RETREAT:
			ParseRetreat(key);
			break;
		default:
			SyntaxError(L"Misused keyword.");
	}

}

void ParseSay(SymbolTableKey& key, FILE* fp) {
	GetNextToken(fp);
	if(CurToken != Token::TOKEN_STRING) {
		SyntaxError(L"Expected string after \"Say\".");
	}
	
	auto ref = SymbolTable.find(key);
	if(ref == SymbolTable.end()){
		SyntaxError(key + L" is not declared.");
	}

	if(ref->second.type != VariableType::CHARACTER){
		SyntaxError(key + L" is not a character.");
	}

	ActionTableItem item;
	item.type = ActionType::SAY; 
	item.data.push_back(key);
	item.data.push_back(std::wstring(StringConstantValue));
	ActionTable.push_back(item);
}

void ParseUseScene(FILE* fp){
	GetNextToken(fp);
	if(CurToken != Token::TOKEN_IDENTIFIER) {
		SyntaxError(L"Expected identifier after \"UseScene\".");
		return;
	}

	ActionTableItem item;
	item.type = ActionType::USESCENE;
	item.data.push_back(std::wstring(StringIdentifierValue));

	ActionTable.push_back(item);
}

void ParseEnter(FILE* fp){
	//数据储存格式 第一个wstring：角色标识符。第二个wstring：位置（左/右）。
	GetNextToken(fp);
	if (CurToken != Token::TOKEN_IDENTIFIER) {
		SyntaxError(L"Expected identifier after \"Enter\".");
		return;
	}

	std::wstring id1(StringIdentifierValue);

	GetNextToken(fp);
	if (CurToken == Token::TOKEN_IDENTIFIER) {
		ActionTableItem item1;
		item1.type = ActionType::ENTER;
		item1.data.push_back(id1);
		item1.data.push_back(L"L");
		ActionTable.push_back(item1);

		std::wstring id2(StringIdentifierValue);
		ActionTableItem item2;
		item2.type = ActionType::ENTER;
		item2.data.push_back(id2);
		item2.data.push_back(L"R");
		ActionTable.push_back(item2);
	}
	else if (CurToken == Token::TOKEN_KEYWORD) {
		if (KeywordValue == Keyword::L) {
			ActionTableItem item1;
			item1.type = ActionType::ENTER;
			item1.data.push_back(id1);
			item1.data.push_back(L"L");
			ActionTable.push_back(item1);
		}
		else if (KeywordValue == Keyword::R) {
			ActionTableItem item1;
			item1.type = ActionType::ENTER;
			item1.data.push_back(id1);
			item1.data.push_back(L"R");
			ActionTable.push_back(item1);
		}
		else {
			SyntaxError(L"Unexpected token after the identifier.");
		}
	}
}

void ParseExit(FILE* fp)
{
	std::wstring arg1;
	std::wstring arg2;

	GetNextToken(fp);
	if (CurToken == Token::TOKEN_IDENTIFIER) {
		arg1 = StringIdentifierValue;
		arg2 = L"I";
	}
	else if (CurToken == Token::TOKEN_KEYWORD) {
		if (KeywordValue == Keyword::L) {
			arg1 = L"L";
			arg2 = L"K";
		}
		else if (KeywordValue == Keyword::R) {
			arg1 = L"R";
			arg2 = L"K";
		}
		else {
			SyntaxError(L"Unexpected token after \"Exit\".");
		}
	}
	else {
		SyntaxError(L"Unexpected token after \"Exit\".");
		return;
	}

	ActionTableItem item;
	item.type = ActionType::EXIT;
	item.data.push_back(arg1);
	item.data.push_back(arg2);
	ActionTable.push_back(item);
}

void ParseAttack(SymbolTableKey& key)
{
	ActionTableItem item;
	item.type = ActionType::ATTACK;
	item.data.push_back(key);
	ActionTable.push_back(item);
}

void ParseDelay(FILE* fp)
{
	GetNextToken(fp);
	if (CurToken != Token::TOKEN_INTEGER) {
		SyntaxError(L"Expected integer after \"Delay\"");
		return;
	}
	ActionTableItem item;
	item.type = ActionType::DELAY;
	item.data.push_back(std::to_wstring(IntengerConstantValue));
	ActionTable.push_back(item);
}

void ParseRetreat(SymbolTableKey& key)
{
	ActionTableItem item;
	item.type = ActionType::RETREAT;
	item.data.push_back(key);
	ActionTable.push_back(item);
}

void SyntaxError(std::wstring msg) {
	printf("Syntax Error at row %d, col %d: %ls\n", curRow, curCol, msg.c_str());
	++MASSErrors;
}

void SyntaxWarning(std::wstring msg) {
	printf("Syntax Warning at row %d, col %d: %ls\n", curRow, curCol, msg.c_str());
	++MASSWarnings;
}

#include "lexer.h"
#include "syntax.h"
#include <string>
#include <stdio.h>
#include "../CharacterController.h"
#include "../Action.h"
#include "../Symbol.h"

extern wchar_t StringIdentifierValue[ID_LEN_MAX]; //�����ʶ��������
extern wchar_t StringConstantValue[STRING_LEN_MAX]; //�����ַ�������
extern int IntengerConstantValue; //������������
extern double FloatConstantValue; //���渡�㳣��
extern LexerErr ErrCode; //�����������򽫴���ԭ�򱣴�������
extern Keyword KeywordValue; //����ؼ��ʵ�ö��ֵ
int MASSErrors;
int MASSWarnings;
extern int curRow;
extern int curCol;
extern bool loadAssetsNow;

constexpr int AudioTrackLimit = 8;

Token CurToken;

SymbolTableType SymbolTable;
ActionTableType ActionTable;

Token GetNextToken(FILE* fp) {
	CurToken = GetToken(fp);
	return CurToken;
}

void Parse(FILE* fp) {
	if (loadAssetsNow) {
		CharacterControllerInit();
	}
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
	
	ActionTable.push_back(new MASSAction::Finish);
	Symbol::LoadSymbols();
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
		case Keyword::DEFINESOUND:
			ParseDefineSound(fp);
			break;
		case Keyword::PLAYSOUND:
			ParsePlaySound(fp);
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
		//��������
	}

	value.data = data;

	SymbolTable.emplace(key, value);
}

//����0���Ѿ�����������ԵĽ���
//����1��δ��ɽ���
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
		//��������
	}

	value.data = data;

	SymbolTable.emplace(key, value);
}

//����0���Ѿ�����������ԵĽ���
//����1��δ��ɽ���
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

	MASSAction::Voiceover* action = new MASSAction::Voiceover;
	action->content = std::wstring(StringConstantValue);

	ActionTable.push_back(action);
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

	MASSAction::Say* action = new MASSAction::Say;
	action->key = key;
	action->content = std::wstring(StringConstantValue);

	ActionTable.push_back(action);
}

void ParseUseScene(FILE* fp){
	GetNextToken(fp);
	if(CurToken != Token::TOKEN_IDENTIFIER) {
		SyntaxError(L"Expected identifier after \"UseScene\".");
		return;
	}

	MASSAction::UseScene* action = new MASSAction::UseScene;
	action->key = std::wstring(StringIdentifierValue);

	ActionTable.push_back(action);
}

void ParseEnter(FILE* fp){
	//���ݴ����ʽ ��һ��wstring����ɫ��ʶ�����ڶ���wstring��λ�ã���/�ң���
	GetNextToken(fp);
	if (CurToken != Token::TOKEN_IDENTIFIER) {
		SyntaxError(L"Expected identifier after \"Enter\".");
		return;
	}

	std::wstring id1(StringIdentifierValue);

	GetNextToken(fp);
	if (CurToken == Token::TOKEN_IDENTIFIER) {
		MASSAction::Enter* action1 = new MASSAction::Enter;
		action1->key = id1;
		action1->position = L'L';

		ActionTable.push_back(action1);

		std::wstring id2(StringIdentifierValue);
		MASSAction::Enter* action2 = new MASSAction::Enter;
		action2->key = id2;
		action2->position = L'R';

		ActionTable.push_back(action2);
	}
	else if (CurToken == Token::TOKEN_KEYWORD) {
		if (KeywordValue == Keyword::L) {
			MASSAction::Enter* action = new MASSAction::Enter;
			action->key = id1;
			action->position = L'L';

			ActionTable.push_back(action);
		}
		else if (KeywordValue == Keyword::R) {
			MASSAction::Enter* action = new MASSAction::Enter;
			action->key = id1;
			action->position = L'R';

			ActionTable.push_back(action);
		}
		else {
			SyntaxError(L"Unexpected token after the identifier.");
		}
	}
}

void ParseExit(FILE* fp)
{
	std::wstring arg1;
	wchar_t arg2;

	GetNextToken(fp);
	if (CurToken == Token::TOKEN_IDENTIFIER) {
		arg1 = StringIdentifierValue;
		arg2 = L'I';
	}
	else if (CurToken == Token::TOKEN_KEYWORD) {
		if (KeywordValue == Keyword::L) {
			arg1 = L"L";
			arg2 = L'K';
		}
		else if (KeywordValue == Keyword::R) {
			arg1 = L"R";
			arg2 = L'K';
		}
		else {
			SyntaxError(L"Unexpected token after \"Exit\".");
		}
	}
	else {
		SyntaxError(L"Unexpected token after \"Exit\".");
		return;
	}

	MASSAction::Exit* action = new MASSAction::Exit;
	action->id = arg1;
	action->flag = arg2;

	ActionTable.push_back(action);
}

void ParseAttack(SymbolTableKey& key)
{
	MASSAction::Attack* action = new MASSAction::Attack;
	action->id = key;
	ActionTable.push_back(action);
}

void ParseDelay(FILE* fp)
{
	GetNextToken(fp);
	if (CurToken != Token::TOKEN_INTEGER) {
		SyntaxError(L"Expected integer after \"Delay\"");
		return;
	}
	MASSAction::Delay* action = new MASSAction::Delay;
	action->val = IntengerConstantValue;

	ActionTable.push_back(action);
}

void ParseRetreat(SymbolTableKey& key)
{
	MASSAction::Retreat* action = new MASSAction::Retreat;
	action->id = key;

	ActionTable.push_back(action);
}

void SyntaxError(std::wstring msg) {
	printf("Syntax Error at row %d, col %d: %ls\n", curRow, curCol, msg.c_str());
	++MASSErrors;
}

void SyntaxWarning(std::wstring msg) {
	printf("Syntax Warning at row %d, col %d: %ls\n", curRow, curCol, msg.c_str());
	++MASSWarnings;
}

int ParseSoundProperty(SoundDataType* data, FILE* fp)
{
	GetNextToken(fp);
	if (CurToken != Token::TOKEN_KEYWORD) {
		SyntaxError(L"Missing keyword in the property group.");
	}

	switch (KeywordValue) {
	case Keyword::ENDDEFINE:
		return 0;
	case Keyword::FILENAME:
		GetNextToken(fp);
		if (CurToken != Token::TOKEN_STRING) {
			SyntaxError(L"Expected string after \"Filename\".");
			break;
		}
		data->Filename = std::wstring(StringConstantValue);
		break;
	case Keyword::LOOP:
		data->loop = true;
		break;
	case Keyword::NOLOOP:
		data->loop = false;
		break;
	default:
		SyntaxError(L"Unexpected keyword in the property group.");
		break;
	}

	return 1;
}

void ParseDefineSound(FILE* fp)
{
	SymbolTableValue value;
	GetNextToken(fp);
	if (CurToken != Token::TOKEN_IDENTIFIER) {
		SyntaxError(L"Expected identifier after \"DefineSound\".");
		return;
	}

	SymbolTableKey key = StringIdentifierValue;
	value.type = VariableType::SOUND;

	GetNextToken(fp);
	if (CurToken != Token::TOKEN_KEYWORD || KeywordValue != Keyword::WHOSE) {
		SyntaxError(L"Expected keyword \"Whose\" after the identifier.");
		return;
	}

	SoundDataType* data = new SoundDataType;
	while (ParseSoundProperty(data, fp)) {
		//��������
	}

	value.data = data;

	SymbolTable.emplace(key, value);
}

void ParsePlaySound(FILE* fp)
{
	/*ActionTableItem item;
	item.type = ActionType::PLAYSOUND;

	//���ݴ����ʽ ��һ��wstring����ɫ��ʶ�����ڶ���wstring��λ�ã���/�ң���
	GetNextToken(fp);
	if (CurToken != Token::TOKEN_IDENTIFIER) {
		SyntaxError(L"Expected identifier after \"PlaySound\".");
		return;
	}

	std::wstring id(StringIdentifierValue);
	item.data.push_back(id);

	GetNextToken(fp);

	if (CurToken != Token::TOKEN_IDENTIFIER) {
		SyntaxError(L"Please specify a track.");
		return;
	}
	if (IntengerConstantValue > AudioTrackLimit) {
		SyntaxError(L"Track out of limit.");
		return;
	}

	
	item.data.push_back(std::to_wstring(IntengerConstantValue));
	ActionTable.push_back(item);*/
}

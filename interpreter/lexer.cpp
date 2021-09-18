#include <stdio.h>
#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <wctype.h>
#include <malloc.h>
#include "../CharacterController.h"

#pragma warning(disable:4996)

wchar_t StringIdentifierValue[ID_LEN_MAX]; //储存标识符的名称
wchar_t StringConstantValue[STRING_LEN_MAX];
int IntengerConstantValue; //储存整数常量
double FloatConstantValue; //储存浮点常量
LexerErr ErrCode; //若解析错误，则将错误原因保存在这里
Keyword KeywordValue; //保存关键词的枚举值
TokenTableType TokenTable;
int curRow;
int curCol;
bool loadAssetsNow;

const wchar_t* KeywordList[] = {
	L"DefineCharacter",
	L"DefineScene",
	L"Whose",
	L"Name",
	L"Texture",
	L"Say",
	L"Voiceover",
	L"Attack",
	L"EndDefine",
	L"UseScene",
	L"Enter",
	L"L",
	L"R",
	L"Exit",
	L"Delay",
	L"Retreat",
	L"DefineSound",
	L"Filename",
	L"Loop",
	L"NoLoop",
	L"PlaySound"
};

void LexerScan(FILE* fp) {

	//LexerInit(); //初始化词法分析器
	Token token;
	while((token = GetToken(fp)) != Token::TOKEN_EOF) {
		switch(token) {
			case Token::TOKEN_IDENTIFIER: {
				size_t len = wcslen(StringIdentifierValue);
				wchar_t* data = (wchar_t*)calloc(len + 1, sizeof(wchar_t));
				wcscpy(data, StringIdentifierValue);
				TokenTable.push_back({Token::TOKEN_IDENTIFIER, data});
				break;
			}
			case Token::TOKEN_INTEGER: {
				int* data = (int*)malloc(sizeof(int));
				*data = IntengerConstantValue;
				TokenTable.push_back({Token::TOKEN_INTEGER, data});
				break;
			}
			case Token::TOKEN_FLOAT: {
				double* data = (double*)malloc(sizeof(double));
				*data = FloatConstantValue;
				TokenTable.push_back({Token::TOKEN_FLOAT, data});
				break;
			}
			case Token::TOKEN_KEYWORD: {
				Keyword* data = (Keyword*)malloc(sizeof(Keyword));
				*data = KeywordValue;
				TokenTable.push_back({Token::TOKEN_KEYWORD, data});
				break;
			}
			case Token::TOKEN_STRING: {
				size_t len = wcslen(StringConstantValue);
				wchar_t* data = (wchar_t*)calloc(len + 1, sizeof(wchar_t));
				wcscpy(data, StringConstantValue);
				TokenTable.push_back({Token::TOKEN_STRING, data});
				break;
			}
		}
	}
}

Token GetToken(FILE* fp) {
	static wchar_t lastChar = getWideChar(fp);

	while (iswspace(lastChar)) { //跳过空格
		lastChar = getWideChar(fp);
	}

	if (iswalpha(lastChar)) { //碰到字符，就检查是否是关键字。如果不是，就认为是标识符。
		memset(StringIdentifierValue, 0, sizeof(StringIdentifierValue));
		StringIdentifierValue[0] = lastChar;

		int cursor = 1;
		while (iswalnum(lastChar = getWideChar(fp))) { //将字符写入数组
			StringIdentifierValue[cursor] = lastChar;
			++cursor;

			if (cursor == ID_LEN_MAX) { //标识符过长，报错
				ErrCode = LexerErr::TOO_LONG_IDENTIFIER;
			}
		}

		int keywordListSize = sizeof(KeywordList) / sizeof(KeywordList[0]); //在关键词列表中匹配
		int i = 0;
		for (; i < keywordListSize; ++i) {
			if (wcscmp(KeywordList[i], StringIdentifierValue) == 0) {
				KeywordValue = (Keyword)i;
				return Token::TOKEN_KEYWORD;
			}
		}

		return Token::TOKEN_IDENTIFIER;
	}

	if (iswdigit(lastChar) || lastChar == L'.') { //碰到数字或小数点
		wchar_t NumberConstantValue[NUMBER_LEN_MAX]; //以字符串形式储存数字
		memset(NumberConstantValue, 0, sizeof(NumberConstantValue));

		int cursor = 0;
		do { //将数字写入数组
			NumberConstantValue[cursor] = lastChar;

			++cursor;
			if (cursor == NUMBER_LEN_MAX) { //数字过长，报错
				ErrCode = LexerErr::TOO_LONG_NUMBER;
				return Token::TOKEN_ERR;
			}

			lastChar = getWideChar(fp);
		} while (iswalnum(lastChar) || lastChar == L'.');

		if (wcschr(NumberConstantValue, L'.')) { //如果有小数点，则认为是浮点数
			FloatConstantValue = wcstod(NumberConstantValue, 0);
			return Token::TOKEN_FLOAT;
		}
		else { //否则认为是整数
			IntengerConstantValue = _wtoi(NumberConstantValue);
			return Token::TOKEN_INTEGER;
		}
	}

	if (lastChar == L'#') { //忽略注释
		do {
			lastChar = getWideChar(fp);
		} while (lastChar != L'\n' && lastChar != L'\r' && lastChar != WEOF);

		if (lastChar != WEOF) {
			return GetToken(fp);
		}
	}

	if (lastChar == L'\"') { //如果读到双引号，说明一个字符串开始了
		int cursor = 0;
		memset(StringConstantValue, 0, sizeof(StringConstantValue));
		if (loadAssetsNow) {
			while ((lastChar = getWideChar(fp)) != L'\"') { //将数字符串入数组
				if (lastChar == L'\\') { //处理转义符
					lastChar = getWideChar(fp);
					lastChar = Escape(lastChar);
					if (lastChar == 0) {
						ErrCode = LexerErr::MISUSE_ESCAPE_CHAR;
						return Token::TOKEN_ERR;
					}
					StringConstantValue[cursor] = lastChar;
					addToCharacterLibrary(lastChar);
				}
				else {
					StringConstantValue[cursor] = lastChar;
					addToCharacterLibrary(lastChar);
				}

				++cursor;
				if (cursor == STRING_LEN_MAX) { //字符串过长，报错
					ErrCode = LexerErr::TOO_LONG_NUMBER;
					return Token::TOKEN_ERR;
				}
			}
		}
		else {
			while ((lastChar = getWideChar(fp)) != L'\"') { //将数字符串入数组
				if (lastChar == L'\\') { //处理转义符
					lastChar = getWideChar(fp);
					lastChar = Escape(lastChar);
					if (lastChar == 0) {
						ErrCode = LexerErr::MISUSE_ESCAPE_CHAR;
						return Token::TOKEN_ERR;
					}
					StringConstantValue[cursor] = lastChar;
				}
				else {
					StringConstantValue[cursor] = lastChar;
				}

				++cursor;
				if (cursor == STRING_LEN_MAX) { //字符串过长，报错
					ErrCode = LexerErr::TOO_LONG_NUMBER;
					return Token::TOKEN_ERR;
				}
			}
		}
		lastChar = getWideChar(fp);
		return Token::TOKEN_STRING;
	}

	if (lastChar == WEOF) { //读到文件结尾
		return Token::TOKEN_EOF;
	}

	wchar_t c = lastChar;
	lastChar = getWideChar(fp);
	return (Token)c;
}


wchar_t Escape(wchar_t c) {
	switch(c) {
		case L'\"':
			return L'\"';
			break;
		case L'\\':
			return L'\\';
			break;
	}
	return (wchar_t)0;
}

wchar_t getWideChar(FILE* fp)
{
	wchar_t ch = fgetwc(fp);
	if (ch == L'\n') {
		++curRow;
		curCol = 1;
	}
	else {
		++curCol;
	}
	return ch;
}

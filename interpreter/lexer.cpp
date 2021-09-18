#include <stdio.h>
#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <wctype.h>
#include <malloc.h>
#include "../CharacterController.h"

#pragma warning(disable:4996)

wchar_t StringIdentifierValue[ID_LEN_MAX]; //�����ʶ��������
wchar_t StringConstantValue[STRING_LEN_MAX];
int IntengerConstantValue; //������������
double FloatConstantValue; //���渡�㳣��
LexerErr ErrCode; //�����������򽫴���ԭ�򱣴�������
Keyword KeywordValue; //����ؼ��ʵ�ö��ֵ
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

	//LexerInit(); //��ʼ���ʷ�������
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

	while (iswspace(lastChar)) { //�����ո�
		lastChar = getWideChar(fp);
	}

	if (iswalpha(lastChar)) { //�����ַ����ͼ���Ƿ��ǹؼ��֡�������ǣ�����Ϊ�Ǳ�ʶ����
		memset(StringIdentifierValue, 0, sizeof(StringIdentifierValue));
		StringIdentifierValue[0] = lastChar;

		int cursor = 1;
		while (iswalnum(lastChar = getWideChar(fp))) { //���ַ�д������
			StringIdentifierValue[cursor] = lastChar;
			++cursor;

			if (cursor == ID_LEN_MAX) { //��ʶ������������
				ErrCode = LexerErr::TOO_LONG_IDENTIFIER;
			}
		}

		int keywordListSize = sizeof(KeywordList) / sizeof(KeywordList[0]); //�ڹؼ����б���ƥ��
		int i = 0;
		for (; i < keywordListSize; ++i) {
			if (wcscmp(KeywordList[i], StringIdentifierValue) == 0) {
				KeywordValue = (Keyword)i;
				return Token::TOKEN_KEYWORD;
			}
		}

		return Token::TOKEN_IDENTIFIER;
	}

	if (iswdigit(lastChar) || lastChar == L'.') { //�������ֻ�С����
		wchar_t NumberConstantValue[NUMBER_LEN_MAX]; //���ַ�����ʽ��������
		memset(NumberConstantValue, 0, sizeof(NumberConstantValue));

		int cursor = 0;
		do { //������д������
			NumberConstantValue[cursor] = lastChar;

			++cursor;
			if (cursor == NUMBER_LEN_MAX) { //���ֹ���������
				ErrCode = LexerErr::TOO_LONG_NUMBER;
				return Token::TOKEN_ERR;
			}

			lastChar = getWideChar(fp);
		} while (iswalnum(lastChar) || lastChar == L'.');

		if (wcschr(NumberConstantValue, L'.')) { //�����С���㣬����Ϊ�Ǹ�����
			FloatConstantValue = wcstod(NumberConstantValue, 0);
			return Token::TOKEN_FLOAT;
		}
		else { //������Ϊ������
			IntengerConstantValue = _wtoi(NumberConstantValue);
			return Token::TOKEN_INTEGER;
		}
	}

	if (lastChar == L'#') { //����ע��
		do {
			lastChar = getWideChar(fp);
		} while (lastChar != L'\n' && lastChar != L'\r' && lastChar != WEOF);

		if (lastChar != WEOF) {
			return GetToken(fp);
		}
	}

	if (lastChar == L'\"') { //�������˫���ţ�˵��һ���ַ�����ʼ��
		int cursor = 0;
		memset(StringConstantValue, 0, sizeof(StringConstantValue));
		if (loadAssetsNow) {
			while ((lastChar = getWideChar(fp)) != L'\"') { //�����ַ���������
				if (lastChar == L'\\') { //����ת���
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
				if (cursor == STRING_LEN_MAX) { //�ַ�������������
					ErrCode = LexerErr::TOO_LONG_NUMBER;
					return Token::TOKEN_ERR;
				}
			}
		}
		else {
			while ((lastChar = getWideChar(fp)) != L'\"') { //�����ַ���������
				if (lastChar == L'\\') { //����ת���
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
				if (cursor == STRING_LEN_MAX) { //�ַ�������������
					ErrCode = LexerErr::TOO_LONG_NUMBER;
					return Token::TOKEN_ERR;
				}
			}
		}
		lastChar = getWideChar(fp);
		return Token::TOKEN_STRING;
	}

	if (lastChar == WEOF) { //�����ļ���β
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

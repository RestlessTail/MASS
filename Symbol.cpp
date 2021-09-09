#include "Symbol.h"
#include "interpreter/syntax.h"
#include "TextureLoader.h"

extern SymbolTableType SymbolTable;

SceneTableType SceneTable;
ActorTableType ActorTable;
SoundTableType SoundTable;

void Symbol::LoadSymbols()
{
	for (auto s : SymbolTable) {
		switch (s.second.type) {
		case VariableType::CHARACTER: {
			InMemoryActorData data;
			auto dataPtr = ((CharacterDataType*)(s.second.data));
			data.Name = dataPtr->Name;
			unsigned int id;
			TextureLoader::loadTexture(&id, dataPtr->Texture);
			data.Texture = id;
			ActorTable.emplace(s.first, data);
			break;
		}
		case VariableType::SCENE: {
			std::wstring path = ((SceneDataType*)s.second.data)->Texture;
			unsigned int id;
			TextureLoader::loadTexture(&id, path);
			SceneTable.emplace(s.first, id);
			break;
		}
		case VariableType::SOUND: {
			InMemorySoundData data;
			auto dataPtr = ((SoundDataType*)(s.second.data));
			data.Filename = dataPtr->Filename;
			data.loop = dataPtr->loop;
			//data.mix = dataPtr->mix;
			SoundTable.emplace(s.first, data);
			break;
		}
		}
	}
}

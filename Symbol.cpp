#include "Symbol.h"
#include "interpreter/syntax.h"
#include "TextureLoader.h"

extern SymbolTableType SymbolTable;

SceneTableType SceneTable;
ActorTableType ActorTable;
SoundTableType SoundTable;

extern bool loadAssetsNow;

void Symbol::LoadSymbols()
{
	if (loadAssetsNow) {
		for (auto s : SymbolTable) {
			switch (s.second.type) {
			case VariableType::CHARACTER: {
				InMemoryActorData data;
				auto dataPtr = ((CharacterDataType*)(s.second.data));
				data.Name = dataPtr->Name;
				unsigned int id;
				TextureLoader::loadTexture(&id, dataPtr->Texture);
				data.Texture = id;
				data.TexturePath = dataPtr->Texture;
				ActorTable.emplace(s.first, data);
				break;
			}
			case VariableType::SCENE: {
				std::wstring path = ((SceneDataType*)s.second.data)->Texture;
				unsigned int id;
				TextureLoader::loadTexture(&id, path);
				InMemorySceneData data = { id, path };
				SceneTable.emplace(s.first, data);
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
	else {
		for (auto s : SymbolTable) {
			switch (s.second.type) {
			case VariableType::CHARACTER: {
				InMemoryActorData data;
				auto dataPtr = ((CharacterDataType*)(s.second.data));
				data.Name = dataPtr->Name;
				unsigned int id = 0;
				data.Texture = id;
				data.TexturePath = dataPtr->Texture;
				ActorTable.emplace(s.first, data);
				break;
			}
			case VariableType::SCENE: {
				std::wstring path = ((SceneDataType*)s.second.data)->Texture;
				unsigned int id = 0;
				InMemorySceneData data = { id, path };
				SceneTable.emplace(s.first, data);
				break;
			}
			case VariableType::SOUND: {
				/*InMemorySoundData data;
				auto dataPtr = ((SoundDataType*)(s.second.data));
				data.Filename = dataPtr->Filename;
				data.loop = dataPtr->loop;
				//data.mix = dataPtr->mix;
				SoundTable.emplace(s.first, data);*/
				break;
			}
			}
		}
	}
}

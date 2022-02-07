#pragma once
#include <string>
#include <irrKlang.h>

constexpr int AudioTrackLimit = 8;

class MASSSoundSource {
public:
	MASSSoundSource();
	MASSSoundSource(std::wstring& filename);
	bool loop;
	irrklang::ISoundSource* irrSoundSource;
	irrklang::ISound* session;
private:
	std::wstring path;
	
};

class MASSSoundEngine {
public:
	MASSSoundEngine();
	MASSSoundSource* AudioTrack[AudioTrackLimit];
	void AddToTrack(MASSSoundSource* sound, int track);
	void StopTrack(int track);
};
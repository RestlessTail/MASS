#include "Sound.h"
#include "utils.h"
#include "MyArknightsScriptScript.h"

MASSSoundSource::MASSSoundSource()
{
}

MASSSoundSource::MASSSoundSource(std::wstring& filename) {
	irrSoundSource = MyArknightsScriptScript::irrklangSoundEngine->addSoundSourceFromFile(MASSutils::wstring2string(filename).c_str());
	loop = false;
}

MASSSoundEngine::MASSSoundEngine()
{
	for (int i = 0; i < AudioTrackLimit; ++i) {
		AudioTrack[i] = nullptr;
	}
}

void MASSSoundEngine::AddToTrack(MASSSoundSource* sound, int track)
{
	StopTrack(track);
	sound->session = MyArknightsScriptScript::irrklangSoundEngine->play2D(sound->irrSoundSource, sound->loop, true);
	sound->session->setIsPaused(false);
	AudioTrack[track] = sound;
}

void MASSSoundEngine::StopTrack(int track)
{
	if (AudioTrack[track]) {
		AudioTrack[track]->session->stop();
		delete AudioTrack[track];
		AudioTrack[track] = nullptr;
	}
}

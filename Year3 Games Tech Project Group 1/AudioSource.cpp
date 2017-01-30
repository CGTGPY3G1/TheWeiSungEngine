#include "AudioSource.h"
#include "AssetManager.h"

AudioSource::AudioSource() : Component() { }

AudioSource::AudioSource(std::weak_ptr<GameObject> gameObject) : Component(gameObject) { }

AudioSource::~AudioSource() { }

void AudioSource::Init(const std::string & path, const bool & play, const bool & loop) {
	sound.setBuffer(AssetManager::GetInstance().GetSoundBuffer(path));
	if(play) Play();
	SetLooping(loop);
}

void AudioSource::SetVolume(const float & volume) {
	sound.setVolume(volume * 100.0f);
}

float AudioSource::GetVolume() {
	return sound.getVolume() * (1.0f / 100.0f);
}

void AudioSource::SetLooping(const bool & looping) {
	sound.setLoop(looping);
}

bool AudioSource::GetLooping() {
	return sound.getLoop();
}

bool AudioSource::IsPlaying() {
	return sound.getStatus() == sf::Sound::Status::Playing;
}

bool AudioSource::IsPaused() {
	return sound.getStatus() == sf::Sound::Status::Paused;
}

void AudioSource::Play() {
	sound.play();
}

void AudioSource::Stop() {
	sound.stop();
}

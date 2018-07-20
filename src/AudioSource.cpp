#include "AudioSource.h"
#include "AssetManager.h"
namespace WeiSungEngine {
	AudioSource::AudioSource() : Component() {}

	AudioSource::AudioSource(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {}

	AudioSource::~AudioSource() {}

	void AudioSource::SetEnabled(const bool & enabled) {
		if (!enabled) sound.stop();
		Component::SetEnabled(enabled);
	}

	void AudioSource::Init(const std::string & path, const bool & play, const bool & loop) {
		sound.setBuffer(AssetManager::GetInstance().GetSoundBuffer(path));
		if (play) Play();
		SetLooping(loop);
	}

	void AudioSource::SetVolume(const float & volume) {
		sound.setVolume(volume * 100.0f);
	}

	float AudioSource::GetVolume() {
		return sound.getVolume() * (1.0f / 100.0f);
	}

	void AudioSource::SetAttenuation(const float & maxDistance, const float & attenuation) {
		sound.setMinDistance(maxDistance);
		sound.setAttenuation(attenuation);
	}

	void AudioSource::SetPosition(const Vector2 & position) {
		sound.setPosition(position.x, 0.0f, position.y);
	}

	float AudioSource::GetAttenuation() {
		return sound.getAttenuation();
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
		return sound.getStatus() == sf::Sound::Status::Paused || sound.getStatus() == sf::Sound::Status::Stopped;
	}

	void AudioSource::Play() {
		sound.play();
	}

	void AudioSource::Stop() {
		sound.stop();
	}
}
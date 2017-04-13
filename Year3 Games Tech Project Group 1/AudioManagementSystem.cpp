#include "AudioManagementSystem.h"
#include "AudioSource.h"
#include "Transform2D.h"
#include "GameObject.h"

AudioManagementSystem::AudioManagementSystem() : System() {
}

AudioManagementSystem::~AudioManagementSystem() {
}

void AudioManagementSystem::ProcessComponents(std::vector<std::shared_ptr<GameObject>>& gameObjects) {
	const unsigned int mask = GetComponentMask();
	const size_t noOfObjects = gameObjects.size();
	for(size_t i = 0; i < noOfObjects; i++) {
		std::shared_ptr<GameObject> go = gameObjects[i];
		if(go) {
			if(go->GetEnabled()) {
				if(go->HasComponents(mask)) {
					std::shared_ptr<Transform2D> transform = go->GetComponent<Transform2D>().lock();
					std::vector<std::weak_ptr<AudioSource>> audioSources = go->GetComponents<AudioSource>();
					for(std::vector<std::weak_ptr<AudioSource>>::iterator i = audioSources.begin(); i != audioSources.end(); ++i) {
						std::shared_ptr<AudioSource> source = (*i).lock();
						if(source && source->GetEnabled()) {
							source->SetPosition(transform->GetPosition());
						}
					}
				}
			}
		}
	}
}

const unsigned int AudioManagementSystem::GetComponentMask() {
	static const unsigned int mask(ComponentType::COMPONENT_TRANSFORM_2D | ComponentType::COMPONENT_AUDIO_SOURCE);
	return mask;
}

#pragma once
#ifndef WS_AUDIO_MANAGEMENT_SYSTEM_H
#define WS_AUDIO_MANAGEMENT_SYSTEM_H
#include "System.h"
namespace WeiSungEngine {
	class AudioManagementSystem : public System {
	public:
		AudioManagementSystem();
		~AudioManagementSystem();
		void ProcessComponents(std::vector<std::shared_ptr<GameObject>> & gameObjects) override;
		const unsigned int GetComponentMask() override;
	};
}

#endif // !WS_AUDIO_MANAGEMENT_SYSTEM_H

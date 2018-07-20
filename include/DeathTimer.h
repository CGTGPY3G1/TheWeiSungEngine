
#ifndef WS_DEATH_TIMER_H
#define WS_DEATH_TIMER_H
#include "ScriptableComponent.h"
namespace WeiSungEngine {
	class DeathTimer : public ScriptableComponent {
	public:
		DeathTimer();
		DeathTimer(std::weak_ptr<GameObject> gameObject);
		~DeathTimer();
		const ComponentType Type() const override { return COMPONENT_DEATH_TIMER; }
		void SetTime(const float & time);
		float GetTime();
		void Update(const float & deltaTime) override;
		const std::string GetName() const override { return "DeathTimer"; }
		int GetSortOrder() override;
	private:
		float lifeTime = 10.0f;
	};
}
#endif // !WS_DEATH_TIMER_H
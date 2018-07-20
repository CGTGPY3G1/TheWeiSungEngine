
#ifndef WS_SYSTEM_H
#define WS_SYSTEM_H
#include <memory>
#include <vector>
namespace WeiSungEngine {
	class GameObject;
	class System : public std::enable_shared_from_this<System> {
	public:
		virtual ~System() {}
		virtual void ProcessComponents(std::vector<std::shared_ptr<GameObject>> & gameObjects) = 0;
		virtual const unsigned int GetComponentMask() = 0;
		std::weak_ptr<System> GetWeak() { return shared_from_this(); }
	};
}
#endif // !WS_SYSTEM_H

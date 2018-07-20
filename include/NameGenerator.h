
#ifndef WS_NAME_GENERATOR_H
#define WS_NAME_GENERATOR_H
#include <vector>
#include <string>
namespace WeiSungEngine {
	class NameGenerator {
	public:
		static NameGenerator & GetInstance();
		~NameGenerator();
		std::string GetRandomName();
	private:
		NameGenerator();
		std::vector<std::string> firstNames, lastNames;
	};
}
#endif //!WS_NAME_GENERATOR_H

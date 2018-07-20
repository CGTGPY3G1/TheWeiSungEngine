
#ifndef WS_DEBUG_H
#define WS_DEBUG_H
#include <sstream>
#include <vector>
namespace WeiSungEngine {
	enum DebugMessageType {
		DEBUG_TYPE_NEUTRAL = 15,
		DEBUG_TYPE_SUCCESS = 10,
		DEBUG_TYPE_FAILURE_LOW = 14,
		DEBUG_TYPE_FAILURE_MEDIUM = 13,
		DEBUG_TYPE_FAILURE_CRITICAL = 12
	};

	struct DebugMessage {
		DebugMessage(const std::string & debugMessage, const DebugMessageType & debugType, const std::string & time) : message(debugMessage), type(debugType), dateTime(time) {}
		std::string message;
		std::string dateTime;
		DebugMessageType type;
	};

	class Debug {
	public:
		~Debug() {}
		static Debug & GetInstance();
		void Log(const std::string & message, const DebugMessageType & type = DEBUG_TYPE_NEUTRAL);
		void PrintImmediately(const std::string & message, const DebugMessageType & type = DEBUG_TYPE_NEUTRAL);
		void PrintException(const std::exception & e);
		void PrintLog();
	private:
		static std::string GetDateTimeAsString();
		std::vector<DebugMessage> loggedMessages;
	};
}
#endif // !WS_DEBUG_H

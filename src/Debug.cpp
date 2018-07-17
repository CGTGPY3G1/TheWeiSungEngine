#include "Debug.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <time.h>
#include <sstream>
#include <typeinfo> 
#include <exception>
Debug & Debug::GetInstance() {
	static Debug instance;
	return instance;
}

void Debug::Log(const std::string & message, const DebugMessageType & type) {
	loggedMessages.push_back(DebugMessage(message, type, GetDateTimeAsString()));
}

void Debug::PrintImmediately(const std::string & message, const DebugMessageType & type) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, type);
	std::string dateTime = GetDateTimeAsString();
	std::cout << dateTime + " | " + message << std::endl;
}

void Debug::PrintException(const std::exception & e) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, DEBUG_TYPE_FAILURE_CRITICAL);
	std::cout << GetDateTimeAsString() + " | Excepion: " + e.what() << std::endl;
}

void Debug::PrintLog() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for(size_t i = 0; i < loggedMessages.size(); i++) {
		SetConsoleTextAttribute(hConsole, loggedMessages[i].type);
		std::cout << loggedMessages[i].dateTime + " | " + loggedMessages[i].message << std::endl;
	}
	loggedMessages.clear();
}

std::string Debug::GetDateTimeAsString() {
	time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char buffer[100];
	struct tm timeinfo;
	if(localtime_s(&timeinfo, &now) == 0) strftime(buffer, sizeof(buffer), "Date/Time = %d-%m-%Y %H:%M:%S", &timeinfo);
	std::stringstream ss;
	ss << buffer;
	return ss.str();
}

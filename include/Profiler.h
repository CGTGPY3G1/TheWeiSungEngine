
#ifndef PROFILER_H
#define PROFILER_H
#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
namespace WeiSungEngine {
	class Profiler {

	public:
		Profiler(std::string profileName);
		~Profiler();
		void StartProfiling() {
			running = true;
			startTime = std::chrono::steady_clock::now();
		}

		void StopProfiling() {
			endTime = std::chrono::steady_clock::now();
			running = false;
			PublishReport();
		}
		void SetProfileName(std::string profileName) {
			this->profileName = profileName;
		}
	private:
		std::chrono::steady_clock::time_point startTime, endTime;
		std::string profileName;
		bool running;
		unsigned long BuildHash(std::string profileName) {
			int hash = 1;
			for (std::string::iterator i = profileName.begin(); i != profileName.end(); i++) {
				hash = ((hash << 5) + hash) + (*i); /* hash * 33 + c */
			}
			return hash;
		}
		void PublishReport() {
			std::chrono::steady_clock::duration timeSpan = endTime - startTime;
			double elapsedTime = double(timeSpan.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
			std::cout << std::fixed << "Session Name = " << profileName << std::endl << "Time Taken = " << std::setprecision(9) << elapsedTime << std::endl;
		}
	};

	Profiler::Profiler(std::string profileName) {
		this->profileName = profileName;
	}

	Profiler::~Profiler() {
	}
}
#endif // PROFILER_S

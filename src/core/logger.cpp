#include "logger.hpp"
#include "common.h"
#include <cstdarg>

void Logger::Log(const char* msg) {
	std::cout << "[INFO] " << msg << std::endl;
}
void Logger::LogWarning(const char* warning) {
	std::clog << "\033[33m[WARNING] " << warning << "\033[0m" << std::endl;
}

void Logger::LogError(const char* err) {
	std::cerr << "\033[31m[ERROR] " << err << "\033[0m" << std::endl;
}

void Logger::LogException(const std::exception& exception) {
	Logger::LogErrorFormat("'%s' exception: %s", typeid(exception).name(), exception.what());
}

void Logger::LogSystem(const char* msg) {
	std::cout << "\033[1;34m" << msg << "\033[0m" << std::endl;
}
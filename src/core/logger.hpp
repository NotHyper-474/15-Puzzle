#pragma once

#include <iostream>
#include <fmt/printf.h>

namespace Logger {
	void Log(const char* msg);
	void LogWarning(const char* warning);
	void LogError(const char* err);
	void LogException(const std::exception& exception);
	void LogSystem(const char* msg);

	template<typename... TArgs>
	void LogFormat(const char* msg, TArgs&&... args);
	template<typename... TArgs>
	void LogWarningFormat(const char* warning, TArgs&&... args);
	template<typename... TArgs>
	void LogErrorFormat(const char* err, TArgs&&... args);
};

template<typename... TArgs>
void Logger::LogFormat(const char* msg, TArgs&&... args) {
	std::cout << "[INFO] " << fmt::sprintf(msg, std::forward<TArgs>(args)...) << "\033[0m\n";
}

template<typename... TArgs>
void Logger::LogWarningFormat(const char* warning, TArgs&&... args) {
	std::clog << "\033[33m[WARNING] " << fmt::sprintf(warning, std::forward<TArgs>(args)...) << "\033[0m\n";
}

template<typename... TArgs>
void Logger::LogErrorFormat(const char* err, TArgs&&... args) {
	std::cerr << "\033[31m[ERROR] " << fmt::sprintf(err, std::forward<TArgs>(args)...) << "\033[0m\n";
}
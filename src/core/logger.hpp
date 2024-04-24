#pragma once

#include <iostream>
#include "common.h"

struct Logger {
	Logger(Logger&) = delete;
	void operator=(const Logger&) = delete;

	static void Log(const char* msg);
	static void LogFormat(const char* msg, ...);
	static void LogWarning(const char* warning);
	static void LogWarningFormat(const char* warning, ...);
	static void LogError(const char* err);
	static void LogErrorFormat(const char* err, ...);
	static void LogException(const std::exception& exception);
	static void LogSystem(const char* msg);


};
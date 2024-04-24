#include "logger.hpp"
#include "common.h"

#include <stdarg.h>
#include <memory>

void Internal_LogFormatGeneric(const char* colorCode, const char* prefix, const char* msg, va_list args) {
	// Format msg first
	int size_Msg = std::vsnprintf(nullptr, 0, msg, args) + 1; // Include trailing character
	if (size_Msg <= 0) { throw std::runtime_error("Error while formatting."); }
	size_t size = static_cast<size_t>(size_Msg);
	auto buf = std::make_unique<char[]>(size);
	std::vsnprintf(buf.get(), size, msg, args);

	// Now cout formatted msg in formatted string
	std::cout << colorCode << '[' << prefix << "] " << buf.get() << "\033[0m" << std::endl;
}

void Logger::Log(const char* msg) {
	std::cout << "[INFO] " << msg << std::endl;
}

void Logger::LogFormat(const char* msg, ...) {
	va_list args;

	va_start(args, msg);
	Internal_LogFormatGeneric("", "INFO", msg, args);
	va_end(args);
}

void Logger::LogWarning(const char* warning) {
	std::clog << "\033[33m[WARNING] " << warning << "\033[0m" << std::endl;
}

void Logger::LogWarningFormat(const char* warning, ...) {
	va_list args;
	va_start(args, warning);
	Internal_LogFormatGeneric("\033[33m", "WARNING", warning, args);
	va_end(args);
}

void Logger::LogError(const char* err) {
	std::clog << "\033[31m[ERROR] " << err << "\033[0m" << std::endl;
}

void Logger::LogErrorFormat(const char* err, ...) {
	va_list args;
	va_start(args, err);
	Internal_LogFormatGeneric("\033[31m", "ERROR", err, args);
	va_end(args);
}

void Logger::LogException(const std::exception& exception) {
	Logger::LogErrorFormat("'%s' exception: %s", typeid(exception).name(), exception.what());
}

void Logger::LogSystem(const char* msg) {
	std::cout << "\033[1;34m" << msg << "\033[0m" << std::endl;
}

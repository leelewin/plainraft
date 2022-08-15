#ifndef PLAINRAFT_BASE_LOGGER_H
#define PLAINRAFT_BASE_LOGGER_H

#include "logstream.h"
#include "timestamp.h"

namespace praft {

//编译期计算源文件的basename

class SourceFile {
public:
	SourceFile(const char* filename) 
	  : data_(filename) {
	    const char* slast = strrchr(filename, '/');
		if (slast) {
			data_ = slast + 1;
		}
		size_ = static_cast<int>(strlen(data_));
	}

	const char* data_;
	int size_;

};

class Logger {
public:
	enum LogLevel {
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL,
		NUM_LOG_LEVELS,
	};

	Logger(SourceFile file, int line);
	Logger(SourceFile file, int line, LogLevel level);
	Logger(SourceFile file, int line, LogLevel level, const char* func);
	Logger(SourceFile file, int line, bool toAbort);
	~Logger(); //析构函数中为日志条目增加后缀

	LogStream& stream();

	static LogLevel getLogLevel();
	static void setLogLevel(LogLevel level);

	using OutputFunc = void (*)(const char* msg, int len);
	using FlushFunc = void (*)();
	static void setOutput(OutputFunc);
	static void setFlush(FlushFunc);

private:
	class Impl {
	public:
		Impl(LogLevel level, int old_errno, const SourceFile &file, int line);
		void formatTime();
		void addLogSuffix();

		Timestamp time_;
		LogStream stream_;
		LogLevel level_;
		int line_;
		SourceFile basename_;
	};

	Impl impl_;

};

extern Logger::LogLevel g_loglevel;

#define LOG_TRACE if (Logger::getLogLevel() <= Logger::TRACE) \
	Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (Logger::getLogLevel() <= Logger::DEBUG) \
	Logger(__FILE__, __LINE__, Logger::DEBUG, __func__).stream()
#define LOG_INFO if (Logger::getLogLevel() <= Logger::INFO) \
	Logger(__FILE__, __LINE__).stream()
#define LOG_WARN Logger(__FILE__, __LINE__, Logger::WARN).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::ERROR).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__, Logger::FATAL).stream()
#define LOG_SYSERR Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL Logger(__FILE__, __LINE__, false).stream()

}

#endif // PLAINRAFT_BASE_LOGGER_H

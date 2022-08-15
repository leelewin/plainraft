#include "base/logger.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

namespace praft {

__thread char t_time[64];
__thread time_t t_last_second;

__thread pid_t t_tid = 0;
__thread char t_tidStr[32];

Logger::LogLevel initLogLevel() {
	return Logger::TRACE;
}

Logger::LogLevel g_loglevel = initLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] = {
	" TRACE ",
	" DEBUG ",
	" INFO ", 
	" WARN ",
	" ERROR ",
	" FATAL ",
};

void defaultOutput(const char* msg, int len) {
	size_t n = fwrite(msg, 1, len, stdout);
	if (n <= 0) {
		perror("fwrite fail");
		abort();
	}
}

void defaultFlush() {
	fflush(stdout);
}

Logger::LogLevel Logger::getLogLevel() {
	return g_loglevel;

}

void Logger::setLogLevel(LogLevel level) {
	g_loglevel = level;
}

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;
Logger::Logger(SourceFile file, int line) 
	: impl_(INFO, 0, file, line) {

}

Logger::Logger(SourceFile file, int line, LogLevel level) 
	: impl_(level, 0, file, line) {

}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func) 
	: impl_(level, 0, file, line) {
	impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, bool toAbort) 
	: impl_(toAbort ? FATAL : ERROR, errno, file, line) {

}

Logger::~Logger() {
	impl_.addLogSuffix();
	const LogStream::Buffer& buff(stream().buffer());
	g_output(buff.data(), buff.length());
	if (impl_.level_ == FATAL) {
		g_flush();
		abort();
	}
}

LogStream& Logger::stream() {
	return impl_.stream_;
}



void Logger::setOutput(OutputFunc out) {
	g_output = out;
}

void Logger::setFlush(FlushFunc flush) {
	g_flush = flush;
}

//Impl 

Logger::Impl::Impl(LogLevel level, int old_errno, const SourceFile &file, int line) 
	: time_(Timestamp::now()),
	  stream_(),
	  level_(level),
	  line_(line),
	  basename_(file) 
{
    formatTime();
	//缓存tid tidStr
	if (t_tid == 0) {
		t_tid = static_cast<pid_t>(::syscall(SYS_gettid));
		int len = snprintf(t_tidStr, sizeof(t_tidStr), " %6d", t_tid);
		if (len < 0) {
			perror("snprinf fail");
			abort();
		}
	}
	stream_ << t_tidStr;
	stream_ << LogLevelName[level];
	if (old_errno != 0) {
		//TODO
	}

}

//优化技巧：1s内没有变化则重用年月日-时分秒，注意__thread的使用
void Logger::Impl::formatTime() {
	time_t mcroseconds_since_epoch = time_.getMicroSecondsSinceEpoch();
	time_t seconds = mcroseconds_since_epoch / Timestamp::kMicroSecondsPerSecond;
	time_t mcroseconds = mcroseconds_since_epoch % Timestamp::kMicroSecondsPerSecond;
	if (seconds != t_last_second) {
		t_last_second = seconds;
		struct tm t;
		localtime_r(&seconds, &t);
		int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d", 
							t.tm_year+1900, t.tm_mon+1, t.tm_mday,
							t.tm_hour, t.tm_min, t.tm_sec);
		if (len < 0) {
			perror("snprintf fail");
			abort();
		}

	}
	//TODO:考虑时区的影响
	stream_ << t_time;
	stream_ << "." << static_cast<int>(mcroseconds);


}

inline
LogStream& operator<< (LogStream &stream, const SourceFile &v) {
	stream.append(v.data_, v.size_);
	return stream;
}

void Logger::Impl::addLogSuffix() {
	stream_ << " - " << basename_ << ':' << line_ << '\n';

}

}

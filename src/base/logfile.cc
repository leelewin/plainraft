#include "base/logfile.h"
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>

namespace praft {

LogFile::LogFile(const std::string& basename,
				 long roll_size,
				 bool thread_safe,   //日志线程数若为1，可以设置为false
				 int flush_interval) 
    : basename_(basename), 
      roll_size_(roll_size),
	  thread_safe_(thread_safe), 
	  flush_interval_(flush_interval),
	  locker_(), 
	  start_of_period_(0),
	  last_roll_(0),
	  last_flush_(0),
	  file_(nullptr) {
	
	assert(basename.find('/') == std::string::npos);
	rollFile();
}

LogFile::~LogFile() = default;

void LogFile::append(const char *logline, int len) {
	if (thread_safe_) {
		std::lock_guard<std::mutex> scope_locker(locker_);
		appendUnlocked(logline, len);
	} else {
		appendUnlocked(logline, len);
	}
}

void LogFile::flush() {
	file_->flush();
}

bool LogFile::rollFile() {
	time_t now = 0;
	std::string filename = getLogFileName(basename_, &now);
	time_t this_period = now / kRollByDay * kRollByDay;

	if (now > last_roll_) {  //小于1s是不会滚动的
		start_of_period_ = this_period;
		last_roll_ = now;
		last_flush_ = now;
		file_.reset(new AppendOnlyFile(filename));
		return true;
	}
	return false;
}

void LogFile::appendUnlocked(const char *logline, int len) {
	file_->append(logline, len);
	if (file_->getWrittenBytes() > roll_size_) {
		rollFile();
	} else {
		time_t now = time(nullptr);
		time_t this_period = now / kRollByDay * kRollByDay;
		//每24小时滚动一次日志
		if (this_period > start_of_period_) {
			rollFile();
		} else if (now - last_flush_ > flush_interval_) {
			last_flush_ = now;
			flush();
		}
	}
}

std::string LogFile::getLogFileName(const std::string &basename, time_t *time) {
	std::string filename(basename);
	struct timeval tv;
	int ret = gettimeofday(&tv, nullptr);
	assert(ret == 0);
	*time = tv.tv_sec;
	struct tm tm_time;
	localtime_r(time, &tm_time);
	char time_str[32];
	snprintf(time_str, sizeof(time_str), ".%4d%02d%02d-%08ld.", 
					tm_time.tm_year+1900, tm_time.tm_mon+1,
					tm_time.tm_mday, tv.tv_usec);

	filename += time_str;
	filename += std::to_string(getpid());
	filename += ".log";
	return filename;
}

}







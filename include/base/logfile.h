#ifndef PLAINRAFT_BASE_LOGFILE_H
#define PLAINRAFT_BASE_LOGFILE_H

#include "appendfile.h"

#include <string>
#include <memory>
#include <mutex>

namespace praft {

/**
 * @brief  实现日志文件的滚动记录
 */
class LogFile {
public:
	LogFile(const std::string& basename,
			long roll_size,
			bool thread_safe = false,   //日志线程数若为1，可以设置为false
			int flush_interval = 3);    //check_every_n有啥用,我觉忽略就可以
	//禁止拷贝
	LogFile(const LogFile &) = delete;
	LogFile& operator= (const LogFile &) = delete;

	~LogFile();

	void append(const char *logline, int len);
	void flush();
	bool rollFile();

private:
	void appendUnlocked(const char *logline, int len);

	static std::string getLogFileName(const std::string &basename, time_t *time); //静态的原因:不改变对象的状态

	const std::string basename_;
	const long roll_size_;
	const bool thread_safe_;
	const int flush_interval_;

	std::mutex locker_;
	time_t start_of_period_; //当天零点 对于北京时间为8点
	time_t last_roll_;
	time_t last_flush_;
	std::unique_ptr<AppendOnlyFile> file_;

	static const int kRollByDay = 24 * 60 * 60;

};

}

#endif  // PLAINRAFT_BASE_LOGFILE_H
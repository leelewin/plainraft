#include "base/timestamp.h"
#include <sys/time.h>

namespace praft {

Timestamp::Timestamp(time_t microseconds_since_epoch) 
	: microseconds_since_epoch_(microseconds_since_epoch) {
	//none
}

time_t Timestamp::getMicroSecondsSinceEpoch() const {
	return microseconds_since_epoch_;
}

Timestamp Timestamp::now() {
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return Timestamp(tv.tv_sec * kMicroSecondsPerSecond + tv.tv_usec);
}

std::string Timestamp::formatStr(bool show_microsecond) {
	char date_time[64];
	time_t now = microseconds_since_epoch_ / kMicroSecondsPerSecond;
	struct tm time_tm;
	localtime_r(&now, &time_tm);
	if (show_microsecond) { //显示微秒
		time_t microsecond = microseconds_since_epoch_ % kMicroSecondsPerSecond;
		snprintf(date_time, sizeof(date_time), "%04d%02d%02d-%02d:%02d:%02d.%9ld", 
				time_tm.tm_year+1900, time_tm.tm_mon+1,time_tm.tm_mday, time_tm.tm_hour,
				time_tm.tm_min, time_tm.tm_sec, microsecond);
	} else {
		snprintf(date_time, sizeof(date_time), "%04d%02d%02d-%02d:%02d:%02d", 
				time_tm.tm_year+1900, time_tm.tm_mon+1,time_tm.tm_mday, time_tm.tm_hour,
				time_tm.tm_min, time_tm.tm_sec);
	}
	return date_time;

}

double timeDifference(const Timestamp &high, const Timestamp &low) {
	int64_t diff = high.getMicroSecondsSinceEpoch() - low.getMicroSecondsSinceEpoch();
	return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

}
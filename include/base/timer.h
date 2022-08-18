#ifndef PLAINRAFT_BASE_TIMER_H
#define PLAINRAFT_BASE_TIMER_H

#include <memory>
#include "base/time_wheel.h"
#include "base/timestamp.h"

namespace praft {

class Timer {
public:
	Timer();
	Timer(const Timer &) = delete;
	Timer& operator=(const Timer &) = delete;
	//absolute timing
	void timeAt(const Timestamp &time, const TimerCallBack &cb);
	// @brief  run callback after @c delay seconds
	void timeAfter(uint32_t delay, const TimerCallBack &cb);
	//run callback every @c interval seconds
	void timeEvery(uint32_t interval, const TimerCallBack &cb);


private:
	std::unique_ptr<TimeWheel> time_wheel_;

};

}


#endif   // PLAINRAFT_BASE_TIMER_H
#ifndef PLAINRAFT_BASE_TIMER_H
#define PLAINRAFT_BASE_TIMER_H

#include <memory>
#include "base/time_wheel.h"

namespace praft {

class Timer {
public:




private:
	std::unique_ptr<TimeWheel> time_wheel_;



};

}


#endif   // PLAINRAFT_BASE_TIMER_H
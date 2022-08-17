#ifndef PLAINTRAFT_BASE_TIME_WHEEL_H
#define PLAINTRAFT_BASE_TIME_WHEEL_H

#include <vector>
#include <memory>
#include <functional>

namespace praft {

typedef std::function<void()> TimerCallBack;

//precision is 10ms，take a turn need 512 * 10ms = 5120ms = 5.12s
//can take any turn
class TimeWheel {
public:
	TimeWheel();
	TimeWheel(const TimeWheel &) = delete;
	TimeWheel& operator=(const TimeWheel &) = delete;
	~TimeWheel();
	 // @param  timeout          default ms
	 // @param  cb               callback func to run when tick
	void addTWItem(uint32_t timeout, TimerCallBack cb);
	void tick();

private:
	//time wheel item
	struct TWItem {
		uint32_t rotation; //take effect after this turns
		uint16_t time_slot; //which slot belong to
		TimerCallBack callback;
		TWItem* prev;
		TWItem* next;
		TWItem(uint32_t rot, uint16_t ts) 
			: rotation(rot), time_slot(ts), 
				prev(nullptr), next(nullptr) { }
	};
	void deleteTWItem(TWItem* item_ptr);
	//slot number
	static const uint16_t kSlotNum = 512;
	//slot interval
	//default is 10 ms
	static const uint16_t kIntervalMs = 10;
	std::vector<TWItem*> slots;
	uint16_t current_slot;

};


}

#endif  // PLAINTRAFT_BASE_TIME_WHEEL_H
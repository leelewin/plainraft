#include "base/time_wheel.h"

namespace praft {

TimeWheel::TimeWheel() : current_slot(0)
{
	//empty
}

TimeWheel::~TimeWheel()
{
	for (TWItem* ptr : slots) {
		TWItem* p = nullptr;
		while (ptr != nullptr) {
			p = ptr->next;
			ptr->next = p->next;
			delete p;
		}
	}
}

void TimeWheel::addTWItem(uint32_t timeout, TimerCallBack cb)
{
	//the number of tick needed
	int ticks = 0;
	//timeout period less than 10, then set 10
	if (timeout < kIntervalMs)
		ticks = 1;
	else 
		ticks = timeout / kIntervalMs; //abandon some ms 

	uint32_t rot = ticks / kSlotNum;
	uint16_t slot_pos = (current_slot + (ticks % kSlotNum)) % kSlotNum;
	
	TWItem* item = new TWItem(rot, slot_pos);
	item->callback = std::move(cb);

	if (!slots[slot_pos]) {
		slots[slot_pos] = item;
	} else {
		item->next = slots[slot_pos];
		slots[slot_pos]->prev = item;
		slots[slot_pos] = item;
	}

}

void TimeWheel::tick()
{

	TWItem* ptr = slots[current_slot];
	while (ptr != nullptr) {
		if (ptr->rotation > 0) {
			--ptr->rotation;
			ptr = ptr->next;
		} else {
			ptr->callback(); //run callbackfunc
			if (ptr == slots[current_slot]) {
				deleteTWItem(ptr);
				ptr = slots[current_slot];
			} else {
				TWItem* tmp = ptr->next;
				deleteTWItem(ptr);
				ptr = tmp;
			}
		}
	}
	current_slot = (++current_slot) % kSlotNum;

}

void TimeWheel::deleteTWItem(TWItem* item_ptr)
{
	if (!item_ptr)
		return;

	int ts = item_ptr->time_slot;

	if (item_ptr == slots[ts]) {
		slots[ts] = slots[ts]->next;
		if (slots[ts])
			slots[ts]->prev = nullptr;
		delete item_ptr;
	} else {
		item_ptr->prev->next = item_ptr->next;
		if (item_ptr->next)
			item_ptr->next->prev = item_ptr->prev;
		delete item_ptr;
	}

}


}
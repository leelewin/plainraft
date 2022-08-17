#include "base/time_wheel.h"
#include <iostream>

void func() {
	std::cout << "tick" << std::endl;
}

int main() {
	praft::TimeWheel wheel;
	wheel.addTWItem(2, func);
	wheel.addTWItem(8, func);
	wheel.addTWItem(21, func);
	wheel.tick();
	wheel.tick();
	wheel.tick();
	wheel.addTWItem(5125, func);
	wheel.addTWItem(5126, func);
	for(int i = 3; i <= 542; ++i) {
		std::cout << i % 512 << " ";
		wheel.tick();
	}
}
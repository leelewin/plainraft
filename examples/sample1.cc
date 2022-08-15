#include "praft.h"

#include <memory>
#include <iostream>


int main() {
	//node1文件夹必须是已经建立好的
	praft::Options options("127.0.0.1:123454,127.0.0.2:12673,127.0.0.3:13469", "127.0.1.2", 1238, "./node1");
	std::shared_ptr<praft::PRaft> pp = praft::PRaft::open(options);

	if (pp == nullptr) {
		std::cout << "fail" << std::endl;
	} else {
		std::cout << "ok" << std::endl;
	}





	return 0;
}
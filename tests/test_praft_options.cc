#include "praft_options.h"
#include <iostream>

int main() {
	std::string members("127.0.0.1:10023,127.0.0.2:10045,127.0.0.3:10026");
	praft::Options options(members, "127.0.1.1", 12233, "./node11");
	std::cout << options.toString();

	return 0;
}
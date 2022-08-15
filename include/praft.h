#ifndef PLAINRAFT_PRAFT_H
#define PLAINRAFT_PRAFT_H

#include <string>
#include <memory>

#include "praft_options.h"

namespace praft {

class PRaft {
public:
	virtual ~PRaft();

	//if any error occur, a nagative value is returned, typically -1
	virtual int write(const std::string &key, const std::string &value) = 0;
	virtual int read(const std::string &key, std::string* value) = 0;

	static std::shared_ptr<PRaft> open(const Options &options);

};

}


#endif // PLAINRAFT_PRAFT_H
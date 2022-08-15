#ifndef PLAINRAFT_BASE_FIXEDBUFFER_H 
#define PLAINRAFT_BASE_FIXEDBUFFER_H

#include <string.h>
#include <string>

namespace praft {

const int kSmallBuffSize = 4000;
const int kLargeBuffSize = 4000 * 1000;

template<int SIZE>
class FixedBuffer {
public:
	FixedBuffer() : current_(buff_) {
		//none
	}
	FixedBuffer(const FixedBuffer &) = delete;
	FixedBuffer& operator= (const FixedBuffer &) = delete;

	void append(const char* buff, size_t len) { //buff满了就丢弃
		if (static_cast<size_t>(avail()) > len) {
			memcpy(current_, buff, len);
			current_ += len;
		}
	}

	const char* data() const {
		return buff_;
	}
	int length() const {
		return static_cast<int>(current_ - buff_);
	}
	int avail() const {
		return static_cast<int>(end() - current_);
	}
	void reset() {
		current_ = buff_;
	}
	void binzero() {
		memset(buff_, 0, SIZE);
	}
	std::string toString() const {
		return std::string(buff_, length());
	}

private:
	const char* end() const {
		return buff_ + sizeof(buff_);
	}
	char buff_[SIZE];
	char* current_;

};

}

#endif  // PLAINRAFT_BASE_FIXEDBUFFER_H

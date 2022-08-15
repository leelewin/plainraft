#include "base/logstream.h"

namespace praft {

LogStream::LogStream() : buffer_() { 

}
void LogStream::append(const char* data, int len) {
	buffer_.append(data, len);
}

const LogStream::Buffer& LogStream::buffer() const {
	return buffer_;
}

void LogStream::resetBuffer() {
	buffer_.reset();
}

LogStream& LogStream::operator<< (bool v) {
	buffer_.append(v ? "1" : "0", 1);
	return *this;
}

LogStream& LogStream::operator<< (char v) {
	buffer_.append(&v, 1);
	return *this;
}

LogStream& LogStream::operator<< (int v) {
	if (buffer_.avail() >= kMaxNumericSize) {
		char str[kMaxNumericSize];
		int len = snprintf(str, kMaxNumericSize, "%d", v);
		buffer_.append(str, len);
	}
	return *this;
}

LogStream& LogStream::operator<< (uint64_t v) {
	if (buffer_.avail() >= kMaxNumericSize) {
		char str[kMaxNumericSize];
		int len = snprintf(str, kMaxNumericSize, "%ld", v);
		buffer_.append(str, len);
	}
	return *this;
}

LogStream& LogStream::operator<< (double v) {
	if (buffer_.avail() >= kMaxNumericSize) {
		char str[kMaxNumericSize];
		int len = snprintf(str, kMaxNumericSize, "%.12g", v);
		buffer_.append(str, len);
	}
	return *this;
}

LogStream& LogStream::operator<< (const char* str) {
	if (str) {
		buffer_.append(str, strlen(str));

	} else {
		buffer_.append("(null)", 6);
	}
	return *this;
}

LogStream& LogStream::operator<< (const std::string &v) {
	buffer_.append(v.c_str(), v.size());
	return *this;
}

}
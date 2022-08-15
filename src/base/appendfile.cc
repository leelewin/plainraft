#include "base/appendfile.h"

namespace praft {

AppendOnlyFile::AppendOnlyFile(const char *name) 
	: fd_(nullptr), writtenBytes_(0) {
	fd_ = ::fopen(name, "ae");
	assert(fd_ != nullptr);
}

AppendOnlyFile::AppendOnlyFile(const std::string &name) 
	: AppendOnlyFile(name.c_str()) {
	//empty
}

AppendOnlyFile::~AppendOnlyFile() {
	::fclose(fd_);

}

void AppendOnlyFile::append(const char *data, size_t len) {
	size_t written = ::fwrite(data, 1, len, fd_);
	assert(written == len);
	writtenBytes_ += written;
}

void AppendOnlyFile::flush() {
	::fflush(fd_);
}

long AppendOnlyFile::getWrittenBytes() {
	return writtenBytes_;

}

}




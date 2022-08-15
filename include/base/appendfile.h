#ifndef PLAINRAFT_BASE_APPENDFILE_H
#define PLAINRAFT_BASE_APPENDFILE_H

#include <stdio.h>
#include <string>
#include <assert.h>

namespace praft {

//非线程安全
class AppendOnlyFile {
public:
	explicit AppendOnlyFile(const char *name);
	explicit AppendOnlyFile(const std::string &name);
	//阻止复制
	AppendOnlyFile(const AppendOnlyFile &) = delete;
	AppendOnlyFile& operator= (const AppendOnlyFile &) = delete;

	~AppendOnlyFile();

	void append(const char *data, size_t len);
	void flush();
	long getWrittenBytes();

private:
	//使用默认的全缓冲大小4KB
	//后面优化可以自定义更大的缓冲，进一步减少系统调用的次数
	FILE* fd_;
	long writtenBytes_;


};

}

#endif  //PLAINRAFT_BASE_APPENDFILE_H
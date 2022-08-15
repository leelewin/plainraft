#ifndef PLAINRAFT_BASE_ASYNCLOGGING_H
#define PLAINRAFT_BASE_ASYNCLOGGING_H
#include "fixedbuffer.h"

#include <memory>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

namespace praft {

/**
 * @brief  双缓冲区异步日志管理
 */
class AsyncLogging {
public:
	explicit AsyncLogging(const std::string &basename,
				 long roll_size = 500*1024*1024,
				 int flush_interval = 3);
	//禁止拷贝
	AsyncLogging(const AsyncLogging &) = delete;
	AsyncLogging& operator= (const AsyncLogging &) = delete;

	~AsyncLogging();

	void append(const char *log, size_t len);


private:
	void threadFunc();
	void stopThread();

	const std::string basename_;
	const long roll_size_;
	const int flush_interval_;

	std::atomic<bool> running_;
	std::thread thread_;
	std::mutex mutex_;
	std::condition_variable cond_;
	typedef FixedBuffer<kLargeBuffSize> LargeBuffer;
	typedef std::unique_ptr<LargeBuffer> BufferPtr;
	BufferPtr current_buffer_;
	BufferPtr next_buffer_;
	std::vector<BufferPtr> buffers_;

};

}

#endif  //PLAINRAFT_BASE_ASYNCLOGGING_H




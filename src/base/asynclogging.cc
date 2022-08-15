#include "base/asynclogging.h"
#include "base/logfile.h"
#include "base/timestamp.h"
#include <functional>

namespace praft {

AsyncLogging::AsyncLogging(const std::string &basename,
						   long roll_size,
						   int flush_interval)
	: basename_(basename), 
	  roll_size_(roll_size),
	  flush_interval_(flush_interval),
	  running_(true),
	//   thread_(&AsyncLogging::threadFunc, this), //两种写法
      thread_(std::bind(&AsyncLogging::threadFunc, this)),
	  mutex_(),
	  cond_(),
	  current_buffer_(new LargeBuffer),
	  next_buffer_(new LargeBuffer),
	  buffers_() {

	current_buffer_->binzero();
	next_buffer_->binzero();
	buffers_.reserve(16);
}

AsyncLogging::~AsyncLogging() {
	stopThread();
}

void AsyncLogging::append(const char *log, size_t len) {
	std::lock_guard<std::mutex> locker(mutex_);
	if (current_buffer_->avail() > static_cast<int>(len)) {
		current_buffer_->append(log, len);
	} 
	else {
		buffers_.push_back(std::move(current_buffer_));
		if (next_buffer_)
			current_buffer_ = std::move(next_buffer_); //移动后next_buffer_置null
		else {
			current_buffer_.reset(new LargeBuffer);
		}
		current_buffer_->append(log, len);
		cond_.notify_all();
	}
}

void AsyncLogging::threadFunc() {
	BufferPtr temp_buff1(new LargeBuffer);
	BufferPtr temp_buff2(new LargeBuffer);
	temp_buff1->binzero();
	temp_buff2->binzero();
	LogFile output(basename_, roll_size_, false, flush_interval_);
	std::vector<BufferPtr> buffers_to_write;
	buffers_to_write.reserve(16);

	while (running_) {
		std::unique_lock<std::mutex> lk(mutex_);
		//不管有没有唤醒，每flush_interval_秒写一次磁盘
		cond_.wait_for(lk, std::chrono::seconds(flush_interval_));
		buffers_.push_back(std::move(current_buffer_));
		current_buffer_ = std::move(temp_buff1);
		buffers_to_write.swap(buffers_);
		if (!next_buffer_) {
			next_buffer_ = std::move(temp_buff2);
		}
		lk.unlock();
		//异常情况
		//size超过25,丢弃掉size-2大小的log
		if (buffers_to_write.size() > 25) {
			char message[256];
			snprintf(message, sizeof(message), "drop log message at %s, %zu larger buffers",
						Timestamp::now().formatStr(false).c_str(), buffers_to_write.size()-2);
			output.append(message, strlen(message));
			buffers_to_write.erase(buffers_to_write.begin()+2, buffers_to_write.end());
		}
		for (auto &buffer : buffers_to_write) {
			output.append(buffer->data(), buffer->length());
		}
		if (buffers_to_write.size() > 2) {
			buffers_to_write.resize(2);
		}
		if (!temp_buff1) { //提高写命中率？？
			temp_buff1 = std::move(buffers_to_write.back());
			buffers_to_write.pop_back();
								  //解引用获取它的对象，调用它的方法
			temp_buff1->reset(); //与temp_buff1.reset()区分
		}
		if (!temp_buff2) {
			temp_buff2 = std::move(buffers_to_write.back());
			buffers_to_write.pop_back();
			temp_buff2->reset();
		}
		buffers_to_write.clear();
		buffers_to_write.reserve(16);
		output.flush();
	}
	output.flush();
}

void AsyncLogging::stopThread() {
	running_ = false;
	cond_.notify_all();
	thread_.join();
}

}







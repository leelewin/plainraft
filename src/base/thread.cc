#include "base/thread.h"

namespace praft {

Thread::Thread() 
	: should_stop_(false),
	  running_(false)
{
	//empty
}

Thread::~Thread()
{
	//empty
}

int Thread::start()
{
	std::lock_guard<std::mutex> lock(running_mutex_);
	should_stop_ = false;
	if (!running_) {
		running_ = true;
		thread_ = std::move(std::thread(&thread_func, this));
	}
	return 0;
}

int Thread::stop()
{
	std::lock_guard<std::mutex> lock(running_mutex_);
	should_stop_ = true;
	if (running_) {
		running_ = false;
		thread_.join();
	}
	return 0;
}

bool Thread::get_should_stop()
{
	return should_stop_.load();
}

void Thread::set_should_step(bool val)
{
	should_stop_.store(val);
}

bool Thread::is_running()
{
	return running_;
}

std::string Thread::get_thread_name()
{
	return thread_name_;
}

void Thread::set_thread_name(const std::string &name)
{
	thread_name_ = name;
}



}
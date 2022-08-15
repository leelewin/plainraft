#ifndef PLAINRAFT_BASE_THREAD_H
#define PLAINRAFT_BASE_THREAD_H

#include <memory>
#include <thread>
#include <mutex>
#include <string>
#include <atomic>

namespace praft {

class Thread {
public:
	Thread();
	Thread(const Thread &) = delete;
	Thread& operator=(const Thread &) = delete;
	virtual ~Thread();

	virtual int start();
	virtual int stop();

	bool get_should_stop();
	void set_should_step(bool val = true);

	bool is_running();

	std::string get_thread_name();
	void set_thread_name(const std::string &name);

protected:
	std::atomic<bool> should_stop_;
private:
	virtual void thread_func() = 0;

	std::mutex running_mutex_;
	bool running_;
	std::thread thread_;
	std::string thread_name_;


};


}

#endif  // PLAINRAFT_BASE_THREAD_H




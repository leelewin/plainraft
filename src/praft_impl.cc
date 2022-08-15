#include "praft_impl.h"


#include "base/logger.h"
#include "base/asynclogging.h"

namespace praft {

std::shared_ptr<PRaft> PRaft::open(const Options &options) 
{
	std::unique_ptr<PRaftImpl> impl(new PRaftImpl(options));
	int ret = impl->init();
	if (ret == -1) {
		return nullptr;
	}
	return impl;
}

PRaft::~PRaft()
{
	//empty
}

PRaftImpl::PRaftImpl(const Options &options)
  : options_(options),
	db_(nullptr),
	logs_and_states_(nullptr),
	context_(nullptr),
	raft_log_(nullptr),
	raft_state_(nullptr),
	state_machine_(nullptr)
{
	//empty
}

PRaftImpl::~PRaftImpl() 
{
	//TODO
}

AsyncLogging g_asynclog("log");
void output(const char* msg, int len) {
	g_asynclog.append(msg, len);
}

int PRaftImpl::init()
{
	//async log start
	Logger::setOutput(output);
	Logger::setLogLevel(Logger::DEBUG);

	//create and set db
	rocksdb::Options options;
	options.create_if_missing = true;
	//can use more thread to accelerate flush immemtable
	//by option max_background_flush
	rocksdb::DB* raw_db = nullptr;
	rocksdb::Status status = rocksdb::DB::Open(options, options_.path + "/db", &raw_db);
	if (!status.ok()) {
		LOG_INFO << "open rocksdb failed. path: " << options_.path << "/db";
		return -1;
	}
	db_ = std::shared_ptr<rocksdb::DB>(raw_db);
	rocksdb::DB* raw_logs_states = nullptr;
	status = rocksdb::DB::Open(options, options_.path + "/log_state", &raw_logs_states);
	if (!status.ok()) {
		LOG_INFO << "open rocksdb failed. path: " << options_.path << "/log_state";
		return -1;
	}
	logs_and_states_ = std::shared_ptr<rocksdb::DB>(raw_logs_states);
	LOG_INFO << "create and set db and log_state db";

	//init or recovery from crash init  context
	raft_log_ = std::make_shared<RaftLog>(logs_and_states_);
	raft_state_ = std::make_shared<RaftState>(logs_and_states_);
	raft_state_->init();
	context_ = std::make_shared<PRaftContext>(options_);
	context_->recoveryFrom(raft_state_);
	LOG_INFO << "init context success";

	// apply to state machine init
	state_machine_ = std::make_unique<PRaftStateMachine>(context_, raft_log_, raft_state_, db_);
	LOG_INFO << "sync apply state machine init";

	//









	



	

	return 0;

}


int PRaftImpl::write(const std::string &key, const std::string &value) 
{
	return -1;

}

int PRaftImpl::read(const std::string &key, std::string *value) 
{
	return -1;

}





}
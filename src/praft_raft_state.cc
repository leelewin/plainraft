#include "praft_raft_state.h"

#include "base/logger.h"

namespace praft {

RaftState::RaftState(std::shared_ptr<rocksdb::DB> &db) 
	: db_(db)
{
	//empty
}

void RaftState::init()
{
	// init or recovery init
	if (getCurrentTerm() == 0)
		setCurrentTerm(0);
	if (getVotedForIp() == "")
		setVotedForIp("");
	if (getVotedForPort() == 0)
		setVotedForPort(0);
	if (getCommitIndex() == 0)
		setCommitIndex(0);
	if (getLastApplied() == 0)
		setLastApplied(0);
}

uint64_t RaftState::getCurrentTerm()
{
	std::string value;
	rocksdb::Status s = db_->Get(rocksdb::ReadOptions(), kCurrentTerm, &value);
	if (s.IsNotFound()) {
		return 0;
	}
	return static_cast<uint64_t>(std::stoul(value));
}

void RaftState::setCurrentTerm(const uint64_t term)
{
	std::string value = std::to_string(term);
	rocksdb::Status s = db_->Put(rocksdb::WriteOptions(), kCurrentTerm, value);
	if (!s.ok()) {
		LOG_INFO << "write term to db fail. term is " << term;
		return;
	}
	return;
}

std::string RaftState::getVotedForIp()
{
	std::string value;
	rocksdb::Status s = db_->Get(rocksdb::ReadOptions(), kVotedForIp, &value);
	if (s.IsNotFound()) {
		return "";
	}
	return value; 
}

void RaftState::setVotedForIp(const std::string &ip)
{
	rocksdb::Status s = db_->Put(rocksdb::WriteOptions(), kVotedForIp, ip);
	if (!s.ok()) {
		LOG_INFO << "write votedfor ip to db fail. ip is " << ip;
		return;
	}
	return;
}

int RaftState::getVotedForPort()
{
	std::string value;
	rocksdb::Status s = db_->Get(rocksdb::ReadOptions(), kVotedForPort, &value);
	if (s.IsNotFound()) {
		return 0;
	}
	return std::stoi(value);
}

void RaftState::setVotedForPort(const int port)
{
	std::string value = std::to_string(port);
	rocksdb::Status s = db_->Put(rocksdb::WriteOptions(), kVotedForPort, value);
	if (!s.ok()) {
		LOG_INFO << "write votedfor port to db fail. port is " << port;
		return;
	}
	return;
}

uint64_t RaftState::getCommitIndex()
{
	std::string value;
	rocksdb::Status s = db_->Get(rocksdb::ReadOptions(), kCommitIndex, &value);
	if (s.IsNotFound()) {
		return 0;
	}
	return static_cast<uint64_t>(std::stoul(value));
}

void RaftState::setCommitIndex(const uint64_t index)
{
	std::string value = std::to_string(index);
	rocksdb::Status s = db_->Put(rocksdb::WriteOptions(), kCommitIndex, value);
	if (!s.ok()) {
		LOG_INFO << "write commit index to db fail. commit index is " << index;
		return;
	}
	return;
}

uint64_t RaftState::getLastApplied() 
{
	std::string value;
	rocksdb::Status s = db_->Get(rocksdb::ReadOptions(), kLastApplied, &value);
	if (s.IsNotFound()) {
		return 0;
	}
	return static_cast<uint64_t>(std::stoul(value));

}

void RaftState::setLastApplied(const uint64_t index)
{
	std::string value = std::to_string(index);
	rocksdb::Status s = db_->Put(rocksdb::WriteOptions(), kLastApplied, value);
	if (!s.ok()) {
		LOG_INFO << "write last applied to db fail. last applied index is " << index;
		return;
	}
	return;
}

}
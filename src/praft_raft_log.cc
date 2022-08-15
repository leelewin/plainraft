#include "praft_raft_log.h"

#include "base/logger.h"

namespace praft {

RaftLog::RaftLog(std::shared_ptr<rocksdb::DB> &db) 
	: db_(db), mutex_(), last_log_index_(0)
{
	// when recovery from crash 
	rocksdb::Iterator* it = db_->NewIterator(rocksdb::ReadOptions());
	it->SeekToLast();
	if (it->Valid()) {
		// need to skip five state, when recovery,
		// because log and state store in same rocksdb, and
		// map key is in order
		it->Prev();  
		it->Prev();
		it->Prev();
		it->Prev();
		it->Prev();
		if (it->Valid()) {
			last_log_index_ = static_cast<uint64_t>(stoul(it->key().ToString()));
		}
	}

}

RaftLog::~RaftLog() 
{
	//empty
}

// @return last log index that entry is appended success
uint64_t RaftLog::append(const std::vector<const Entry*> &entries) 
{
	std::lock_guard<std::mutex> lock(mutex_);
	rocksdb::WriteBatch batch;
	rocksdb::Status s;
	for (uint32_t i = 0; i < entries.size(); ++i) {
		std::string buff;
		entries[i]->SerializeToString(&buff);
		last_log_index_++; // first index is 1
		s = batch.Put(std::to_string(last_log_index_), buff);
		if (!s.ok()) {
			LOG_INFO << "push to batch fail in log index = " << last_log_index_;
			last_log_index_ -= i;
			return last_log_index_;
		}
	}
	s = db_->Write(rocksdb::WriteOptions(), &batch);
	if (!s.ok()) {
		last_log_index_ -= entries.size();
		LOG_INFO << "write batch to db fail in log index = " << last_log_index_;
	}
	return last_log_index_;
}

// @return return -1 if fail
int RaftLog::getEntry(uint64_t index, Entry* entry) 
{
	if (index > last_log_index_) {
		LOG_INFO << "index out of range, index : " << index;
		return -1;
	}
	std::string value;
	rocksdb::Status s = db_->Get(rocksdb::ReadOptions(), std::to_string(index), &value);
	if (!s.ok()) {
		LOG_INFO << "get entry from db fail, key = " << index;
		return -1;
	}
	entry->ParseFromString(value);
	return 0;
}

uint64_t RaftLog::getLastLogIndex() const 
{
	return last_log_index_;
}

// @return return -1 if fail
int RaftLog::getLastLogTermAndIndex(uint64_t* index, uint64_t* term) 
{
	*index = last_log_index_;
	std::string value;
	rocksdb::Status s = db_->Get(rocksdb::ReadOptions(), std::to_string(last_log_index_), &value);
	if (!s.ok()) {
		LOG_INFO << "get value from db fail. log index is " << last_log_index_;
		return -1;
	}
	Entry entry;
	entry.ParseFromString(value);
	*term = entry.term();
	return 0;
}

// delete entries in the follower's log after that point,
// for consistency  
// @return return -1 if fail 
int RaftLog::truncateFromPoint(uint64_t index) 
{
	for (; last_log_index_ >= index; --last_log_index_) {
		rocksdb::Status s = db_->Delete(rocksdb::WriteOptions(), std::to_string(last_log_index_));
		if (!s.ok()) {
			LOG_INFO << "delete key in db fail. key = " << last_log_index_;
			return -1;
		}
	}
	return 0;
}


}
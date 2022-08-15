#ifndef PLAINRAFT_PRAFT_RAFT_H
#define PLAINRAFT_PRAFT_RAFT_H

#include <memory>
#include <vector>

#include "rocksdb/db.h"
#include "raft.pb.h"

namespace praft {

// persistance raft log 
// need to consider inet byte order convert ??
class RaftLog {
public:
	explicit RaftLog(std::shared_ptr<rocksdb::DB> &db);
	RaftLog(const RaftLog &) = delete;
	RaftLog& operator=(const RaftLog &) = delete;
	~RaftLog();

	// @return last log index that entry is appended success
	uint64_t append(const std::vector<const Entry*> &entries);

	// @return return -1 if fail
	int getEntry(uint64_t index, Entry* entry);

	uint64_t getLastLogIndex() const;

	// @return return -1 if fail
	int getLastLogTermAndIndex(uint64_t* term, uint64_t* index);

	// @return return -1 if fail 
	int truncateFromPoint(uint64_t index);

private:
	// rocksdb::DB* db_;
	std::shared_ptr<rocksdb::DB> db_;
	std::mutex mutex_;
	uint64_t last_log_index_;

};

}

#endif // PLAINRAFT_PRAFT_RAFT_H

#ifndef PLAINRAFT_PRAFT_RAFT_STATE_H
#define PLAINRAFT_PRAFT_RAFT_STATE_H

#include <string>
#include <memory>
#include "rocksdb/db.h"

namespace praft {

static const std::string kCurrentTerm = "kCurrentTerm";
static const std::string kVotedForIp = "kVotedForIp";
static const std::string kVotedForPort = "kVotedForPort";
static const std::string kCommitIndex = "kCommitIndex";
static const std::string kLastApplied = "kLastApplied";

// persistance states
class RaftState {
public:
	explicit RaftState(std::shared_ptr<rocksdb::DB> &db);
	RaftState(const RaftState &) = delete;
	RaftState& operator=(const RaftState &) = delete;
	~RaftState() = default;

	void init();

	uint64_t getCurrentTerm();
	void setCurrentTerm(const uint64_t term);

	std::string getVotedForIp();
	void setVotedForIp(const std::string &ip);

	int getVotedForPort();
	void setVotedForPort(const int port);

	uint64_t getCommitIndex();
	void setCommitIndex(const uint64_t index);

	uint64_t getLastApplied();
	void setLastApplied(const uint64_t index);

private:
	std::shared_ptr<rocksdb::DB> db_;

};


}

#endif  // PLAINRAFT_PRAFT_RAFT_STATE_H
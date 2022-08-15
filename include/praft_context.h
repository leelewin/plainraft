#ifndef PLAINRAFT_PRAFT_CONTEXT_H
#define PLAINRAFT_PRAFT_CONTEXT_H

#include <string>
#include <mutex>

#include "rocksdb/db.h"
#include "praft_options.h"
#include "praft_raft_state.h"

namespace praft {

enum class Role {
	kFollower = 0,
	kCandidate = 1,
	KLeader = 2,
};

struct PRaftContext {

	Role role;
	Options options;

	uint64_t current_term;
	//use ip:port locate a candidate
	std::string votedfor_ip; 
	int votedfor_port;
	//for redirect leader
	std::string leader_ip;
	int leader_port;
	uint64_t commit_index;  //TODO：atomic?
	uint64_t last_applied;

	//used for protect 
	std::mutex global_mutex;

	PRaftContext(const Options &options);
	void becomeFollower(uint64_t new_term, const std::string &ip_leader, int port_leader);
	void becomeCandidate();
	void becomeLeader();
	// void recoveryFrom(RaftState* state);
	void recoveryFrom(std::shared_ptr<RaftState> state);

};

}
#endif // PLAINRAFT_PRAFT_CONTEXT_H





#ifndef PLAINRAFT_STATE_MACHINE_APPLY_H
#define PLAINRAFT_STATE_MACHINE_APPLY_H

#include "praft_context.h"
#include "praft_raft_log.h"

#include "rocksdb/db.h"

namespace praft {

//sync apply command to state machine
//TODO: async ?
class PRaftStateMachine {
//TODO: PRaftSateMachine  set to abstract base class, user can implement their own design
public:
	PRaftStateMachine(std::shared_ptr<PRaftContext> &context, 
					  std::shared_ptr<RaftLog> &raft_log, 
					  std::shared_ptr<RaftState> &raft_state, 
					  std::shared_ptr<rocksdb::DB> &db);
	PRaftStateMachine(const PRaftStateMachine &) = delete;
	PRaftStateMachine& operator=(const PRaftStateMachine &) = delete;
	~PRaftStateMachine();
	int applyToSateMachine();

private:
	int apply();

	// PRaftContext* context_;
	// RaftLog* raft_log_;
	// RaftState* raft_state_;
	// rocksdb::DB* db_;
	std::shared_ptr<PRaftContext> context_;
	std::shared_ptr<RaftLog> raft_log_;
	std::shared_ptr<RaftState> raft_state_;
	std::shared_ptr<rocksdb::DB> db_;

};


}


#endif  // PLAINRAFT_STATE_MACHINE_APPLY_H
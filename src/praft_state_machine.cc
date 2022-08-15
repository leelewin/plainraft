#include "praft_state_machine.h"

namespace praft {

PRaftStateMachine::PRaftStateMachine(std::shared_ptr<PRaftContext> &context, 
					std::shared_ptr<RaftLog> &raft_log, 
					std::shared_ptr<RaftState> &raft_state, 
					std::shared_ptr<rocksdb::DB> &db)
	: context_(context),
	  raft_log_(raft_log),
	  raft_state_(raft_state),
	  db_(db)
{
	//empty
}

PRaftStateMachine::~PRaftStateMachine()
{
	//empty
}

int PRaftStateMachine::applyToSateMachine()
{
	//TODO: if commitindex > lastapplied  apply
	return 0;

}

int PRaftStateMachine::apply() 
{
	return 0;

}

}

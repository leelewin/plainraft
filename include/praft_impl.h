#ifndef PLAINRAFT_PRAFT_IMPL_H
#define PLAINRAFT_PRAFT_IMPL_H

#include "praft.h"

#include <memory>

#include "praft_context.h"
#include "praft_raft_log.h"
#include "praft_raft_state.h"
#include "praft_state_machine.h"

#include "rocksdb/db.h"

namespace praft {

class PRaftImpl : public PRaft {
public:
	explicit PRaftImpl(const Options &options);
	PRaftImpl(const PRaftImpl &) = delete;
	PRaftImpl& operator=(const PRaftImpl &) = delete;
	virtual ~PRaftImpl();

	int init();

	virtual int write(const std::string &key, const std::string &value) override;
	virtual int read(const std::string &key, std::string *value) override;

private:
	Options options_;
	//db for state machine
	std::shared_ptr<rocksdb::DB> db_;
	//used for persisting log and server states data
	std::shared_ptr<rocksdb::DB> logs_and_states_;

	// PRaftContext* context_;
	std::shared_ptr<PRaftContext> context_;
	// persistence raft log entry and state
	std::shared_ptr<RaftLog> raft_log_;
	std::shared_ptr<RaftState> raft_state_;

	//apply entry to state machine
	std::unique_ptr<PRaftStateMachine> state_machine_;

	



	

	



};




};


#endif // PLAINRAFT_PRAFT_IMPL_H












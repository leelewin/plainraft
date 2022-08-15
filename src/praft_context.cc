#include "praft_context.h"

namespace praft {

PRaftContext::PRaftContext(const Options &options) 
	: role(Role::kFollower),
	  options(options),
	  current_term(0),
	  votedfor_ip(""),
	  votedfor_port(0),
	  leader_ip(""),
	  leader_port(0),
	  commit_index(0),
	  last_applied(0),
	  global_mutex() 
{
	//none
}

void PRaftContext::becomeFollower(uint64_t new_term, const std::string &ip_leader, 
								  int port_leader)
{
	std::lock_guard<std::mutex> locker(global_mutex);
	current_term = new_term;
	votedfor_ip.clear(); //votedfor_ip应该清空
	votedfor_port = 0;
	leader_ip = ip_leader;
	leader_port = port_leader;
	role = Role::kFollower;
}

void PRaftContext::becomeCandidate() 
{
	std::lock_guard<std::mutex> locker(global_mutex);
	current_term += 1;
	role = Role::kCandidate;
	votedfor_ip = options.local_ip;
	votedfor_port = options.local_port;
	leader_ip.clear();
	leader_port = 0;

}

void PRaftContext::becomeLeader() 
{
	std::lock_guard<std::mutex> locker(global_mutex);
	leader_ip = options.local_ip;
	leader_port = options.local_port;
	role = Role::KLeader;
}

void PRaftContext::recoveryFrom(std::shared_ptr<RaftState> state) 
{
	current_term = state->getCurrentTerm();
	votedfor_ip = state->getVotedForIp();
	votedfor_port = state->getVotedForPort();
	commit_index = state->getCommitIndex();
	last_applied = state->getLastApplied();

}

}
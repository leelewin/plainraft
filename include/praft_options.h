#ifndef PLAINRAFT_PRAFT_OPTIONS_H
#define PLAINRAFT_PRAFT_OPTIONS_H

#include <vector>
#include <string>

namespace praft {

//parse and store the cluster member
//plan: restart ,then read from configration
class Options {
public:
	Options(const std::string &cluster, 
	        std::string _local_ip, int _local_port, 
			const std::string &_path);

	std::string toString();

	std::vector<std::string> cluster_members;
	std::string local_ip;
	int local_port;
	std::string path;
private:
	//parse from "ip1:port1, ip2:port2" to "ip1:port1" "ip2:port2"
	void parseClusterString(const std::string &cluster_string, char sep);
};


}

#endif // PLAINRAFT_PRAFT_OPTIONS_H

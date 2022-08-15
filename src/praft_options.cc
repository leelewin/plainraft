#include "praft_options.h"

#include <string.h>

namespace praft {

Options::Options(const std::string &cluster, std::string _local_ip, int _local_port, const std::string &_path) 
  : local_ip(_local_ip),
    local_port(_local_port),
	path(_path)
{
	parseClusterString(cluster, ',');
}

std::string Options::toString()
{
	int buff_size = 1024;
	char buff[buff_size];
	std::string cluster;
	for (auto str : cluster_members) {
		cluster += str + ",";
	}
	snprintf(buff, buff_size, "cluster member : %s local_ip : %s local_port %d", 
	                                cluster.c_str(),
									local_ip.c_str(),
									local_port);
	return std::string(buff);

}


void Options::parseClusterString(const std::string &cluster_string, char sep)
{
	size_t pre_pos = cluster_string.find_first_not_of(sep, 0);
	size_t pos = cluster_string.find(sep, pre_pos);
	while (pre_pos != std::string::npos || pos != std::string::npos) {
		cluster_members.push_back(cluster_string.substr(pre_pos, pos));
		pre_pos = cluster_string.find_first_not_of(sep, pos);
		pos = cluster_string.find(sep, pre_pos);
	}
}

}

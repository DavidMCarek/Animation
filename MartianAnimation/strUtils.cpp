#include "strUtils.h"

std::vector<std::string> split(const std::string s, std::string delim) {

	std::vector<std::string> vec;
	size_t last = 0;
	size_t next = 0; 

	while ((next = s.find(delim, last)) != std::string::npos) { 
		vec.push_back(s.substr(last, next - last));
		last = next + delim.length(); 
	} 
	vec.push_back(s.substr(last));

	return vec;
}
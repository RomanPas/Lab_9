#pragma once

#include <vector>
#include <unordered_set>
#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"

struct Options {
	std::vector<std::string> includePaths;
	std::unordered_set<std::string> excludePaths;
	std::size_t levelScannig = 0;
	std::size_t minFileSize = 1;
	std::vector<std::string> masks;
	std::size_t blockSize = 7;
	bool crc16 = false;
	bool ok = false;
};

Options parsingArgs(int argc, char** argv);
std::istream& operator>>(std::istream& in, std::unordered_set<std::string>& paths);
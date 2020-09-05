#include "options.h"

#include <iostream>

using namespace std;
using namespace boost::filesystem;
using namespace boost::program_options;

Options parsingArgs(int argc, char** argv) {
	Options options;
	try
	{
		options_description desc{ "Options" };
		desc.add_options()
			("include,i", value<vector<string>>(), "Include paths for scaning")
			("exclude,e", value<unordered_set<string>>(), "Exclude paths from scaning")
			("level,l", value<long>(), "Scan nesting level, 0 - without nested")
			("min-size,s", value<long>(), "Minimum file size in bytes for scaning")
			("block-size,b", value<long>(), "Block size in bytes for reading file")
			("hash-type,t", value<string>(), "Hash crc16 or crc32")
			("help,h", "Show help")
			;

		variables_map var;
		store(parse_command_line(argc, argv, desc), var);
		notify(var);

		if (var.find("help") != var.end()) {
			cout << desc << "\n";
			return {};
		}

		if (var.find("include") != var.end()) {
			options.includePaths = var["include"].as<vector<string>>();
		}
		else {
			throw error("Include scan paths not defined");
		}

		if (var.find("exclude") != var.end()) {
			options.excludePaths = var["exclude"].as<unordered_set<string>>();
		}

		if (var.find("level") != var.end()) {
			long nLevel = var["level"].as<long>();
			if (nLevel < 0) {
				throw error("Level scaning must be integer");
			}
			options.levelScannig = static_cast<size_t>(nLevel);
		}

		if (var.find("min-size") != var.end()) {
			long nMinSize = var["min-size"].as<long>();
			if (nMinSize < 1) {
				throw error("Minimum file size must be greater then 1");
			}
			options.minFileSize = static_cast<size_t>(nMinSize);
		}

		if (var.find("block-size") != var.end()) {
			long nBlockSize = var["block-size"].as<long>();
			if (nBlockSize < 1) {
				throw error("Minimum file size must be greater then 1");
			}
			options.blockSize = static_cast<size_t>(nBlockSize);
		}

		if (var.find("hash-type") != var.end()) {
			string hash = var["hash-type"].as<string>();
			if (hash == "crc16")
				options.crc16 = true;
			else if (hash == "crc32")
				options.crc16 = false;
			else
				throw error("Unknown hash type");
		}

		options.ok = true;
		return options;
	}
	catch (const exception& exp)
	{
		cout << exp.what() << '\n';
		return options;
	}
}

istream& operator>>(istream& in, unordered_set<string>& paths)
{
	std::string path;
	in >> path;
	paths.insert(move(path));
	return in;
}
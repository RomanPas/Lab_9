#include "bayan_lib.h"

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <fstream>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/crc.hpp>

using namespace std;
using namespace boost::filesystem;

void searchForDuplicate(unordered_map<unsigned long long, list<string>>& data, const Options& options) {
	unordered_map<string, list<int>> hashs;

	for (auto& d : data) {
		hashs.clear();
		for (const auto& name : d.second)
			hashs[name] = {};

		unordered_set<string> result;
		for (auto itA = begin(d.second); itA != end(d.second);) {
			result.insert(*itA);
			for (auto itB = next(itA); itB != end(d.second);) {
				if (compare(*itA, *itB, hashs[*itA], hashs[*itB], options.blockSize, options.crc16)) {
					result.insert(*itB);
					itB = d.second.erase(itB);
				}
				else {
					itB++;
				}
			}
			if (result.size() > 1) {
				for (const auto& name : result)
					cout << name << "\n";
				cout << "\n";
			}
			result.clear();
			itA++;
		}
	}
}

unordered_map<unsigned long long, list<string>> searchFiles(const Options& options) {
	unordered_map<unsigned long long, list<string>> data;

	list<pair<int, path>> paths;
	for (const auto& p : options.includePaths)
		paths.push_back({ 0,{ p } });

	while (!paths.empty()) {
		const int currentLevel = paths.front().first;
		const auto& item = paths.front().second;

		if (options.levelScannig != 0 && options.levelScannig < currentLevel)
			break;

		if (options.excludePaths.find(item.string()) == options.excludePaths.end()) {
			try {
				if (exists(item)) {
					if (is_regular_file(item)) {
						if (file_size(item) >= options.minFileSize)
							data[file_size(item)].push_back(item.string());
					}
					else if (is_directory(item)) {
						for (auto& path : directory_iterator(item)) {
							paths.emplace_back(currentLevel + 1, path);
						}
					}
					else {
						cout << item << " exists, but is not a regular file or directory\n";
					}
				}
				else {
					cout << item << " does not exist\n";
				}
			}
			catch (const filesystem_error& ex) {
				cout << ex.what() << '\n';
			}
		}

		paths.pop_front();
	}

	for (auto it = begin(data); it != end(data);)
		it = it->second.size() == 1 ? data.erase(it) : next(it);

	return data;
}

bool compare(const string& nameA, const string& nameB, list<int>& hashA, list<int>& hashB, const size_t blockSize, bool crc16) {
	unique_ptr<char[]> buff(new char[blockSize + 1]);
	std::ifstream finA(nameA);
	std::ifstream finB(nameB);

	auto itA = hashA.begin();
	auto itB = hashB.begin();

	finA.seekg(blockSize * hashA.size());
	finB.seekg(blockSize * hashB.size());

	int currentHashA = 0;
	int currentHashB = 0;

	bool result = true;

	while (!finA.eof() || itA != hashA.end()) {
		currentHashA = getNextHash(hashA, itA, finA, buff.get(), blockSize, crc16);
		currentHashB = getNextHash(hashB, itB, finB, buff.get(), blockSize, crc16);

		if (currentHashA != currentHashB) {
			result = false;
			break;
		}
	}

	finA.close();
	finB.close();
	return result;
}

int getHashCRC16(const string& str) {
	boost::crc_16_type result;
	result.process_bytes(str.data(), str.length());
	return result.checksum();
}

int getHashCRC32(const string& str) {
	boost::crc_32_type result;
	result.process_bytes(str.data(), str.length());
	return result.checksum();
}

int getNextHash(list<int>& hash, list<int>::iterator& it, std::ifstream& fin, char* buff, const size_t blockSize, bool crc16) {
	int nextHash = 0;
	if (it == hash.end()) {
		fin.get(buff, blockSize + 1);

		int gcount = fin.gcount();
		if (gcount == 0) {
			return nextHash;
		}
		else if (gcount != blockSize) {
			for (int i = gcount; i < blockSize; ++i) {
				buff[i] = '\0';
			}
		}
		nextHash = crc16 ? getHashCRC16(buff) : getHashCRC32(buff);
		hash.push_back(nextHash);
		it = hash.end();
	}
	else {
		nextHash = *it;
		++it;
	}

	return nextHash;
}
#pragma once

#include <list>
#include <string>
#include <unordered_map>

#include "options.h"

std::unordered_map<unsigned long long, std::list<std::string>> searchFiles(const Options& options);

void searchForDuplicate(std::unordered_map<unsigned long long, std::list<std::string>>& data, const Options& options);
bool compare(const std::string& nameA, const std::string& nameB, std::list<int>& hashA, std::list<int>& hashB, const size_t blockSize, bool crc16);
int getHashCRC16(const std::string& block);
int getHashCRC32(const std::string& block);
int getNextHash(std::list<int>& hash, std::list<int>::iterator& it, std::ifstream& fin, char* buff, const size_t blockSize, bool crc16);
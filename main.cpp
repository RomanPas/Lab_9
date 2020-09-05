#include <iostream>
#include <string>

#include "lib/options.h"
#include "lib/bayan_lib.h"

using namespace std;

int main(int argc, char* argv[]) {
	auto options = parsingArgs(argc, argv);
	if (!options.ok) {
		return 1;
	}

	auto data = searchFiles(options);
	searchForDuplicate(data, options);

	return 0;
}
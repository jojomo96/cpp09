#include <iostream>
#include <string>

#include "BitcoinExchange.hpp"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	BitcoinExchange::printResult(argv[1]);
	return 0;
}

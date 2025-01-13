#include "PmergeMe.hpp"
#include <vector>
#include <deque>
#include <algorithm>
#include <random>
#include <sstream>

std::vector<int> validateAndReturnArgs(int argc, char *argv[]) {
	std::vector<int> result;
	for (int i = 1; i < argc; ++i) {
		std::istringstream iss(argv[i]);
		int number;
		if (std::string extra; !(iss >> number) || number < 0 || iss >> extra) {
			std::cerr << "Invalid input: " << argv[i] << " is not a single positive integer." << std::endl;
			return {};
		}
		result.push_back(number);
	}
	return result;
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		const std::vector testInput = {11, 2, 17, 0, 16, 8, 6, 15, 10, 3, 21, 1, 18, 9, 14, 19, 12, 5, 4, 20, 13, 7};

		// Run with vector
		const int run_vector = PmergeMe::runDeque(testInput, true);
		PmergeMe::runVector(testInput, false);

		// Run with deque
		// PmergeMe::runDeque(testInput);
		std::cout << "Global comparison count: " << run_vector << std::endl;
	} else {
		const std::vector<int> input = validateAndReturnArgs(argc, argv);
		if (!input.empty()) {
			int run_deque = PmergeMe::runDeque(input, true);
			PmergeMe::runVector(input);

			std::cout << "Global comparison count: " << run_deque << std::endl;
		}
	}


	// std::vector<int> testInput(21);
	// std::iota(testInput.begin(), testInput.end(), 1); // Fill with 1, 2, ..., 3000
	//
	// // Shuffle the vector to ensure the numbers are in random order
	// std::random_device rd;
	// std::mt19937 g(rd());
	// std::shuffle(testInput.begin(), testInput.end(), g);

	// Print the vector to verify


	return 0;
}

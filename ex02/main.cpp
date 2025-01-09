#include "PmergeMe.hpp"
#include <vector>
#include <deque>
#include <algorithm>
#include <random>
int main() {
	// Test input
	std::vector testInput = {11, 2, 17, 0, 16, 8, 6, 15, 10, 3, 21, 1, 18, 9, 14, 19, 12, 5, 4, 20, 13, 7};

	// std::vector<int> testInput(21);
	// std::iota(testInput.begin(), testInput.end(), 1); // Fill with 1, 2, ..., 3000
	//
	// // Shuffle the vector to ensure the numbers are in random order
	// std::random_device rd;
	// std::mt19937 g(rd());
	// std::shuffle(testInput.begin(), testInput.end(), g);

	// Print the vector to verify


	// Run with vector
	int run_vector = PmergeMe::runVector(testInput);

	// Run with deque
	// PmergeMe::runDeque(testInput);
	std::cout << "Global comparison count: " << run_vector << std::endl;

	return 0;
}

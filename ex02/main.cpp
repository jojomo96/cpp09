#include "PmergeMe.hpp"
#include <vector>
#include <deque>

int main() {
	// Test input
	std::vector testInput = {11, 2, 17, 0, 16, 8, 6, 15, 10, 3, 21, 1, 18, 9, 14, 19, 12, 5, 4, 20, 13, 7};

	// Run with vector
	PmergeMe::runVector(testInput);

	// Run with deque
	// PmergeMe::runDeque(testInput);

	return 0;
}

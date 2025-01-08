#include "PmergeMe.hpp"

#include <deque>
#include <iostream>
#include <vector>


Element::Element(int data, const int depth) : _data(data),  _depth(depth) {
}

Element::Element(const std::shared_ptr<Element> &first, const std::shared_ptr<Element> &second, const int depth)
	: _data(Pair(first, second)), _depth(depth) {
}

int Element::getMaxValue() const { // NOLINT(*-no-recursion)
	if (std::holds_alternative<int>(_data)) {
		return std::get<int>(_data); // Base Case
	}

	// Validate Pair data before proceeding
	if (std::holds_alternative<Pair>(_data)) {
		const auto& [first, second] = std::get<Pair>(_data);

		// Ensure pointers are valid
		if (!first || !second) {
			throw std::runtime_error("Invalid Pair: Null pointer detected");
		}

		// Recursive calls
		return std::max(first->getMaxValue(), second->getMaxValue());
	}

	// Fallback: Should never reach here
	throw std::runtime_error("Element contains unsupported type in _data");
}


void Element::print(const int i) const { // NOLINT(*-no-recursion)
	static constexpr std::array<const char*, 6> colors = {
		"\033[0;31m", // Red
		"\033[0;32m", // Green
		"\033[0;33m", // Yellow
		"\033[0;34m", // Blue
		"\033[0;35m", // Magenta
		"\033[0;36m"  // Cyan
	};
	static auto resetColor = "\033[0m";

	if (std::holds_alternative<int>(_data)) {
		std::cout << colors[i % colors.size()] << std::get<int>(_data) << resetColor << " ";
	} else if (std::holds_alternative<Pair>(_data)) {
		const auto& [first, second] = std::get<Pair>(_data);

		// Validate pointers
		if (!first || !second) {
			std::cerr << colors[i % colors.size()] << "Invalid Pair: Null pointer detected" << resetColor << "\n";
			return;
		}

		std::cout << colors[i % colors.size()] << "( ";
		first->print( i);
		second->print( i);
		std::cout << colors[i % colors.size()] << ")" << resetColor;
	} else {
		std::cerr << colors[i % colors.size()] << "Unsupported type in _data" << resetColor << "\n";
	}
}

std::shared_ptr<Element> makeElement(int data) {
	return std::make_shared<Element>(data);
}

std::shared_ptr<Element> merge(const std::shared_ptr<Element> &first, const std::shared_ptr<Element> &second) {
	const int depth = std::max(first->_depth, second->_depth) + 1;
	return std::make_shared<Element>(first, second, depth);
}

void Element::sortElement() {
	if (std::holds_alternative<int>(_data)) {
		return;
	}

	if (std::holds_alternative<Pair>(_data)) {
		auto& [first, second] = std::get<Pair>(_data);

		if (!first || !second) {
			throw std::runtime_error("Invalid Pair: Null pointer detected");
		}

		if (first->getMaxValue() > second->getMaxValue()) {
			// std::cout << "Swapping elements: " << first->getMaxValue() << " and " << second->getMaxValue() << std::endl;
			++globalComparisonCount;
			std::swap(first, second);
		}
	}
}

inline int PmergeMe::getGlobalComparisonCount() {
	return globalComparisonCount;
}

int PmergeMe::runVector(const std::vector<int>& input) {
	std::vector<std::shared_ptr<Element>> elements;
	parseInput(elements, input);
	run(elements);
	return globalComparisonCount;
}

int PmergeMe::runDeque(const std::vector<int> &input) {
	std::deque<std::shared_ptr<Element>> elements;
	parseInput(elements, input);
	run(elements);
	return globalComparisonCount;
}

std::vector<size_t> PmergeMe::generateJacobsthalIndices(size_t n) {
	std::vector<size_t> result;
	if (n == 0) return result;

	// Standard Jacobsthal starts: J(0)=0, J(1)=1
	// We skip J(0) if you like to begin with 1-based insertion.
	size_t j0 = 0;
	size_t j1 = 1;

	// If pendingChain.size() >= 1, we can always push back `1` (if you want J(1) = 1).
	if (j1 < n) {
		result.push_back(j1); // Insert index=1 first
	}

	// Generate subsequent values while they stay < n
	while (true) {
		size_t j2 = j1 + 2 * j0; // J(n) = J(n-1) + 2*J(n-2)
		if (j2 >= n) {
			break;
		}
		result.push_back(j2);
		j0 = j1;
		j1 = j2;
	}

	// Sort them just to be sure they’re in ascending order
	// (Usually they already come in ascending order, but just in case)
	std::sort(result.begin(), result.end());

	return result;
}


#include "PmergeMe.hpp"

#include <deque>
#include <iostream>
#include <vector>
#include <cmath>


Element::Element(int data, const int depth) : _data(data), _depth(depth) {
}

Element::Element(const std::shared_ptr<Element> &first, const std::shared_ptr<Element> &second, const int depth)
	: _data(Pair(first, second)), _depth(depth) {
}

int Element::getMaxValue() const {
	// NOLINT(*-no-recursion)
	if (std::holds_alternative<int>(_data)) {
		return std::get<int>(_data); // Base Case
	}

	// Validate Pair data before proceeding
	if (std::holds_alternative<Pair>(_data)) {
		const auto &[first, second] = std::get<Pair>(_data);

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


void Element::print(const int i) const {
	// NOLINT(*-no-recursion)
	static constexpr std::array<const char *, 6> colors = {
		"\033[0;31m", // Red
		"\033[0;32m", // Green
		"\033[0;33m", // Yellow
		"\033[0;34m", // Blue
		"\033[0;35m", // Magenta
		"\033[0;36m" // Cyan
	};
	static auto resetColor = "\033[0m";

	if (std::holds_alternative<int>(_data)) {
		std::cout << colors[i % colors.size()] << std::get<int>(_data) << resetColor << " ";
	} else if (std::holds_alternative<Pair>(_data)) {
		const auto &[first, second] = std::get<Pair>(_data);

		// Validate pointers
		if (!first || !second) {
			std::cerr << colors[i % colors.size()] << "Invalid Pair: Null pointer detected" << resetColor << "\n";
			return;
		}

		std::cout << colors[i % colors.size()] << "( ";
		first->print(i);
		second->print(i);
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
		auto &[first, second] = std::get<Pair>(_data);

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

int PmergeMe::runVector(const std::vector<int> &input) {
	std::vector<std::shared_ptr<Element> > elements;
	parseInput(elements, input);
	run(elements);
	return globalComparisonCount;
}

int PmergeMe::runDeque(const std::vector<int> &input) {
	std::deque<std::shared_ptr<Element> > elements;
	parseInput(elements, input);
	run(elements);
	return globalComparisonCount;
}

std::vector<size_t> generateJacobsthalUpTo(size_t maxN) {
	std::vector<size_t> result;
	for (size_t i = 0; ; ++i) {
		size_t jacobsthalNumber = (std::pow(2, i + 1) + std::pow(-1, i)) / 3;
		if (jacobsthalNumber >= maxN) {
			break;
		}
		result.push_back(jacobsthalNumber);
	}

	return result;
}

std::vector<size_t> PmergeMe::generateJacobsthalIndices(const size_t listSize) {
	if (listSize < 1) return {};

	const std::vector<size_t> jNumbers = generateJacobsthalUpTo(listSize);

	if (jNumbers.empty()) return {};

	auto jCurrentIt = jNumbers.begin() + 1;
	auto jPreviousIt = jNumbers.begin();

	std::vector<size_t> indices;
	while (jCurrentIt != jNumbers.end()) {
		for (size_t i = 0; i < *jCurrentIt - *jPreviousIt; ++i) {
			indices.push_back(*jCurrentIt - i - 2);
		}

		std::advance(jPreviousIt, 1);
		std::advance(jCurrentIt, 1);
	}

	return indices;
}

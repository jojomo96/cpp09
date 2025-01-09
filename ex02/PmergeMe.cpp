#include "PmergeMe.hpp"

#include <deque>
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

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
	const auto &[first, second] = std::get<Pair>(_data);
	return std::max(first->getMaxValue(), second->getMaxValue());
}


std::shared_ptr<Element> makeElement(int data) {
	return std::make_shared<Element>(data);
}

std::shared_ptr<Element> merge(const std::shared_ptr<Element> &first, const std::shared_ptr<Element> &second) {
	const int depth = std::max(first->_depth, second->_depth) + 1;
	return std::make_shared<Element>(first, second, depth);
}

void Element::sortElement() {
	if (std::holds_alternative<int>(_data)) return;

	if (auto &[first, second] = std::get<Pair>(_data); first->getMaxValue() > second->getMaxValue()) {
		printSwap(first, second);
		std::swap(first, second);
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

std::vector<size_t> generateJacobsthalUpTo(const size_t maxN) {
	std::vector<size_t> result;
	result.reserve(32);
	for (size_t i = 0; ; ++i) {
		size_t jacobsthalNumber = (std::pow(2, i + 1) + std::pow(-1, i)) / 3;
		result.push_back(jacobsthalNumber);
		if (jacobsthalNumber >= maxN) {
			break;
		}
	}

	return result;
}

std::vector<size_t> PmergeMe::generateJacobsthalIndices(const size_t listSize) {
	const std::vector<size_t> jNumbers = generateJacobsthalUpTo(listSize);

	std::vector<size_t> differences(jNumbers.size());
	std::adjacent_difference(jNumbers.begin(), jNumbers.end(), differences.begin());

	std::vector<size_t> indices;
	indices.reserve(listSize);
	for (size_t i = 1; i < jNumbers.size(); ++i) {
		std::generate_n(std::back_inserter(indices), differences[i], [n = jNumbers[i] - 2]() mutable {
			return n--;
		});
	}

	return indices;
}

size_t PmergeMe::getBoundary(const std::vector<size_t> &indices, size_t value) {
	value += 2;
	for (size_t i = 0; i < indices.size(); ++i) {
		if (indices[i] == value) {
			return i;
		}
	}
	return -1;
}

#pragma region Print functions
void PmergeMe::printInsertion(const std::shared_ptr<Element> &elem, size_t idx,
                              const std::shared_ptr<Element> &boundaryElem, size_t boundaryIdx,
                              const std::string &prefix) {
	if (!printDebug) return;
	std::cout << prefix << "Insert elem b" << idx + 2 << " ";
	elem->print(0);
	std::cout << "from pend idx(" << idx << ") with boundary ";
	if (boundaryElem) {
		boundaryElem->print(0);
	}
	std::cout << "idx " << boundaryIdx << std::endl;
}

void Element::print(const int i) const {
	if (!printDebug) return;

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

void Element::printSwap(const std::shared_ptr<Element> &first, const std::shared_ptr<Element> &second) {
	std::cout << "Swapping elements: ";
	first->print(0);
	std::cout << " and ";
	second->print(0);
	std::cout << std::endl;
	++globalComparisonCount;
}

void PmergeMe::printOddInsertion(const std::shared_ptr<Element>& odd, const std::string &prefix ) {
	if (printDebug) {
		std::cout << "     Insert " << prefix << " elem ";
		odd->print(0);
		std::cout << " into main chain" << std::endl;
	}
}
#pragma endregion

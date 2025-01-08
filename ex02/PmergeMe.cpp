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


void Element::print(const int depth) const { // NOLINT(*-no-recursion)
	if (std::holds_alternative<int>(_data)) {
		// std::cout << std::string(depth, '-') << std::get<int>(_data) << " ";
		std::cout << std::get<int>(_data) << " ";
	}
	else if (std::holds_alternative<Pair>(_data)) {
		const auto& [first, second] = std::get<Pair>(_data);

		// Validate pointers
		if (!first || !second) {
			std::cerr << std::string(depth, '-') << "Invalid Pair: Null pointer detected\n";
			return;
		}

		// std::cout << std::string(depth, '-') << "(";
		std::cout << "( ";
		first->print(depth + 1);
		second->print(depth + 1);
		std::cout << ")";
	}
	else {
		std::cerr << std::string(depth, '-') << "Unsupported type in _data\n";
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
			std::swap(first, second);
		}
	}
}

void PmergeMe::runVector(const std::vector<int>& input) {
	std::vector<std::shared_ptr<Element>> elements;
	parseInput(elements, input);
	run(elements);
}

void PmergeMe::runDeque(const std::vector<int>& input) {
	std::deque<std::shared_ptr<Element>> elements;
	parseInput(elements, input);
	run(elements);
}

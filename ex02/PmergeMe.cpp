#include "PmergeMe.hpp"

#include <iostream>


Element::Element(int data) : _data(data) {
}

Element::Element(const std::shared_ptr<Element> &first, const std::shared_ptr<Element> &second)
	: _data(Pair(first, second)) {
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
		std::cout << std::string(depth, '-') << std::get<int>(_data) << "\n";
	}
	else if (std::holds_alternative<Pair>(_data)) {
		const auto& [first, second] = std::get<Pair>(_data);

		// Validate pointers
		if (!first || !second) {
			std::cerr << std::string(depth, '-') << "Invalid Pair: Null pointer detected\n";
			return;
		}

		std::cout << std::string(depth, '-') << "Pair:\n";
		first->print(depth + 1);
		second->print(depth + 1);
	}
	else {
		std::cerr << std::string(depth, '-') << "Unsupported type in _data\n";
	}
}

std::shared_ptr<Element> makeElement(int data) {
	return std::make_shared<Element>(data);
}

std::shared_ptr<Element> merge(const std::shared_ptr<Element> &first, const std::shared_ptr<Element> &second) {
	return std::make_shared<Element>(first, second);
}



#pragma once
#include <variant>

struct Element;

using Pair = std::pair<std::shared_ptr<Element>, std::shared_ptr<Element>>;

struct Element {
	std::variant<int, Pair> _data;

	explicit Element(int data);

	explicit Element(const std::shared_ptr<Element>& first, const std::shared_ptr<Element>& second);

	[[nodiscard]] int getMaxValue() const;

	void print(int depth = 0) const;
};

std::shared_ptr<Element> makeElement(int data);

std::shared_ptr<Element> merge(const std::shared_ptr<Element>& first, const std::shared_ptr<Element>& second);

class PmergeMe {

};

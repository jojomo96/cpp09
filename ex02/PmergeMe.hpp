
#pragma once
#include <iostream>
#include <variant>
#include <vector>

struct Element;

using Pair = std::pair<std::shared_ptr<Element>, std::shared_ptr<Element>>;

struct Element {
	std::variant<int, Pair> _data;
	int _depth = 0;

	explicit Element(int data, int depth = 0);

	explicit Element(const std::shared_ptr<Element>& first, const std::shared_ptr<Element>& second, int depth);

	[[nodiscard]] int getMaxValue() const;

	void print(int depth = 0) const;

	void sortElement();
};

std::shared_ptr<Element> makeElement(int data);

std::shared_ptr<Element> merge(const std::shared_ptr<Element>& first, const std::shared_ptr<Element>& second);

class PmergeMe {
	template <typename T>
	static void parseInput(T& container, const std::vector<int>& input);

	template <typename T>
	static void print(const T& elements);

	template <typename T>
	static void run(T& elements);

	template <typename T>
	static void sort(T& elements);

	template <typename T>
	static bool mergeIntoPairs(T &elements);

	template <typename T>
	static void swapPairs(T& elements);

public:
	PmergeMe() = delete;
	PmergeMe(const PmergeMe&) = delete;
	PmergeMe(PmergeMe&&) = delete;
	PmergeMe& operator=(const PmergeMe&) = delete;
	PmergeMe& operator=(PmergeMe&&) = delete;

	static void runVector(const std::vector<int>& input);
	static void runDeque(const std::vector<int>& input);
};

template<typename T>
void PmergeMe::parseInput(T &container, const std::vector<int> &input) {
	for (const auto& value : input) {
		container.push_back(makeElement(value));
	}
}

template<typename T>
void PmergeMe::print(const T &elements) {
	for (const auto& element : elements) {
		element->print();
	}
	std::cout << std::endl;
}

template<typename T>
void PmergeMe::run(T &elements) {
	print(elements);
	sort(elements);
}

template<typename T>
void PmergeMe::sort(T &elements) {
	if (mergeIntoPairs(elements)) {
		swapPairs(elements);
		print(elements);
		sort(elements);
	}
}

template<typename T>
bool PmergeMe::mergeIntoPairs(T &elements) {
	bool merged = false;
	for (size_t i = 0; i < elements.size(); i += 1) {
		if (i + 1 < elements.size() && elements[i]->_depth == elements[i + 1]->_depth) {
			elements[i] = merge(elements[i], elements[i + 1]);
			elements.erase(elements.begin() + i + 1);
			merged = true;
		} else {
			elements[i]->_depth += 2;
		}
	}
	return merged;
}

template<typename T>
void PmergeMe::swapPairs(T &elements) {
	for (const auto& element : elements) {
		element->sortElement();
	}
}

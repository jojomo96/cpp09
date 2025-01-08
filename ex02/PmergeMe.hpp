#pragma once
#include <iostream>
#include <variant>
#include <vector>

struct Element;

using Pair = std::pair<std::shared_ptr<Element>, std::shared_ptr<Element> >;

struct Element {
	std::variant<int, Pair> _data;
	int _depth = 0;

	explicit Element(int data, int depth = 0);

	explicit Element(const std::shared_ptr<Element> &first, const std::shared_ptr<Element> &second, int depth);

	[[nodiscard]] int getMaxValue() const;

	void print(int i) const;

	void sortElement();
};

std::shared_ptr<Element> makeElement(int data);

std::shared_ptr<Element> merge(const std::shared_ptr<Element> &first, const std::shared_ptr<Element> &second);

class PmergeMe {
	template<typename T>
	static void parseInput(T &container, const std::vector<int> &input);

	template<typename T>
	static void print(const T &elements);

	template<typename T>
	static void run(T &elements);

	template<typename T>
	static T sort(T &elements);

	template<typename T>
	static bool mergePairs(T &elements);

	template<typename T>
	static void swapPairs(T &elements);

	template<typename T>
	static void splitElementsIntoChains(const T &elements, T &mainChain, T &pendingChain, std::shared_ptr<Element> &odd);

	template<typename T>
	static void sortElementIntoChain(const std::shared_ptr<Element> &elem, T &mainChain);

public:
	PmergeMe() = delete;

	PmergeMe(const PmergeMe &) = delete;

	PmergeMe(PmergeMe &&) = delete;

	PmergeMe &operator=(const PmergeMe &) = delete;

	PmergeMe &operator=(PmergeMe &&) = delete;

	static void runVector(const std::vector<int> &input);

	static void runDeque(const std::vector<int> &input);
};

template<typename T>
void PmergeMe::parseInput(T &container, const std::vector<int> &input) {
	for (const auto &value: input) {
		container.push_back(makeElement(value));
	}
}

template<typename T>
void PmergeMe::print(const T &elements) {
	for (size_t i = 0; i < elements.size(); ++i) {
		elements[i]->print(i);
	}
	std::cout << std::endl;
}

template<typename T>
void PmergeMe::run(T &elements) {
	print(elements);
	sort(elements);
}

template<typename T>
T PmergeMe::sort(T &elements) {
	if (mergePairs(elements)) {
		swapPairs(elements);
		print(elements);
		elements = sort(elements);
	}
	T mainChain;
	T pendingChain;
	std::shared_ptr<Element> odd;

	splitElementsIntoChains(elements, mainChain, pendingChain, odd);
	std::cout << "Main chain: " << std::endl;
	print(mainChain);
	std::cout << "Pending chain: " << std::endl;
	print(pendingChain);
	if (odd) {
		std::cout << "Odd: " << std::endl;
		odd->print(0);
		std::cout << std::endl;
	}
	if (!pendingChain.empty()) {
		for (auto &elem: pendingChain) {
			sortElementIntoChain(elem, mainChain);
		}
	}
	if (odd) {
		sortElementIntoChain(odd, mainChain);
	}

	std::cout << "Sorted main chain: " << std::endl;
	print(mainChain);
	return mainChain;
}

template<typename T>
bool PmergeMe::mergePairs(T &elements) {
	if (elements.empty()) {
		return false;
	}

	int targetDepth = elements[0]->_depth;
	std::cout << "Merging elements with depth " << targetDepth << std::endl;

	T newElements;
	bool merged = false;

	for (size_t i = 0; i < elements.size(); ++i) {
		if (i + 1 < elements.size() && elements[i]->_depth == targetDepth && elements[i + 1]->_depth == targetDepth) {
			newElements.push_back(merge(elements[i], elements[i + 1]));
			++i; // Skip the next element as it has been merged
			merged = true;
		} else {
			newElements.push_back(elements[i]);
		}
	}

	elements = std::move(newElements);
	return merged;
}

template<typename T>
void PmergeMe::swapPairs(T &elements) {
	for (const auto &element: elements) {
		element->sortElement();
	}
}

template<typename T>
void PmergeMe::splitElementsIntoChains(const T &elements, T &mainChain,
                                       T &pendingChain, std::shared_ptr<Element> &odd) {
	int targetDepth = elements[0]->_depth;
	bool isFirstPair = true;

	for (auto &elem: elements) {
		if (targetDepth == elem->_depth) {
			if (std::holds_alternative<int>(elem->_data)) {
				mainChain.push_back(elem);
			} else {
				auto &pairRef = std::get<Pair>(elem->_data);
				auto &left = pairRef.first; // should be the smaller one if swapPairs() was called
				auto &right = pairRef.second; // should be the bigger one

				if (isFirstPair) {
					mainChain.push_back(left);
					mainChain.push_back(right);
					isFirstPair = false;
				} else {
					pendingChain.push_back(left);
					mainChain.push_back(right);
				}
			}
		} else if (targetDepth - 1 == elem->_depth) {
			odd = elem;
		} else {
			mainChain.push_back(elem);
		}
	}
}

template<typename T>
void PmergeMe::sortElementIntoChain(const std::shared_ptr<Element> &elem, T &mainChain) {
	auto it = mainChain.begin();
	while (it != mainChain.end() && (*it)->getMaxValue() < elem->getMaxValue() && (*it)->_depth >= elem->_depth) {
		++it;
	}
	mainChain.insert(it, elem);
}

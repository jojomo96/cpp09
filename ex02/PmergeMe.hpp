#pragma once
#include <iostream>
#include <numeric>
#include <variant>
#include <vector>

struct Element;

inline int globalComparisonCount = 0;

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

	template<class T>
	static void printChains(const T &mainChain, const T &pendingChain, const std::shared_ptr<Element> &odd,
	                        const T &rest);

	template<typename T>
	static void run(T &elements);

	static size_t getBoundary(const std::vector<size_t> &indices, size_t value);

	static void printInsertion(const std::shared_ptr<Element> &elem, size_t idx, const std::shared_ptr<Element> &boundaryElem,
	                           size_t boundaryIdx, const std::string &prefix = "     ");

	template<typename T>
	static T sort(T &elements, T &rest);

	template<typename T>
	static bool mergePairs(T &elements, T &rest);

	template<typename T>
	static void swapPairs(T &elements);

	template<typename T>
	static void splitElementsIntoChains(const T &elements, T &mainChain, T &pendingChain, std::shared_ptr<Element> &odd,
	                                    T &rest);

	template<class T>
	static typename T::iterator sortElementIntoChain(const std::shared_ptr<Element> &elem, T &mainChain,
	                                                 typename T::iterator endBoundary);

public:
	PmergeMe() = delete;

	PmergeMe(const PmergeMe &) = delete;

	PmergeMe(PmergeMe &&) = delete;

	PmergeMe &operator=(const PmergeMe &) = delete;

	PmergeMe &operator=(PmergeMe &&) = delete;

	static int getGlobalComparisonCount();

	static int runVector(const std::vector<int> &input);

	static int runDeque(const std::vector<int> &input);

	static std::vector<size_t> generateJacobsthalIndices(size_t listSize);
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
void PmergeMe::printChains(const T &mainChain, const T &pendingChain, const std::shared_ptr<Element> &odd,
                           const T &rest) {
	std::cout << std::endl;
	std::cout << "Main: ";
	for (size_t i = 0; i < mainChain.size(); ++i) {
		mainChain[i]->print(i);
	}

	if (!pendingChain.empty()) {
		std::cout << " | Pend: ";
		for (size_t i = 0; i < pendingChain.size(); ++i) {
			pendingChain[i]->print(i);
		}
	}
	if (odd) {
		std::cout << " | Odd: ";
		odd->print(0);
	}

	if (!rest.empty()) {
		std::cout << " | Rest: ";
		for (size_t i = 0; i < rest.size(); ++i) {
			rest[rest.size() - 1 - i]->print(i);
		}
	}
	std::cout << std::endl;
}

template<typename T>
void PmergeMe::run(T &elements) {
	print(elements);
	T rest = {};
	sort(elements, rest);
}

template<typename T>
T PmergeMe::sort(T &elements, T &rest) {
	if (mergePairs(elements, rest)) {
		swapPairs(elements);
		printChains(elements, {}, nullptr, rest);
		elements = sort(elements, rest);
	}

	T mainChain;
	T pendingChain;
	std::shared_ptr<Element> odd;

	splitElementsIntoChains(elements, mainChain, pendingChain, odd, rest);

	std::cout << "-------------------------------------------" << std::endl;
	printChains(mainChain, pendingChain, odd, rest);

	if (!pendingChain.empty()) {
		auto jacobIndices = generateJacobsthalIndices(pendingChain.size());

		if (jacobIndices.empty()) {
			std::cout << "Not enough elements in pending chain to generate Jacobsthal indices." << std::endl;
		} else {
			std::cout << "Jacobsthal indices: ";
			for (const size_t idx: jacobIndices) {
				std::cout << idx << " ";
			}
			std::cout << std::endl;
		}

		// Generate a vector of booleans to keep track of which elements have been inserted
		std::vector<bool> inserted(pendingChain.size(), false);

		std::vector<size_t> mainChainIndices(mainChain.size());
		std::iota(mainChainIndices.begin(), mainChainIndices.end(), 0); // Initialize with 0, 1, 2, ...

		auto lastInsertion = mainChain.begin(); // Lower bound for the next insertion

		// Insert the elements at the Jacobsthal indices
		for (size_t idx: jacobIndices) {
			if (idx < pendingChain.size()) {
				size_t boundaryIdx = getBoundary(mainChainIndices, idx);
				printInsertion(pendingChain[idx], idx, mainChain[boundaryIdx], boundaryIdx, "J -> ");
				lastInsertion = sortElementIntoChain(pendingChain[idx], mainChain, mainChain.begin() + boundaryIdx );
				mainChainIndices.insert(mainChainIndices.begin() + (lastInsertion - mainChain.begin()), 0);
				inserted[idx] = true;
			}
		}

		// Insert any leftover elements that were not covered by Jacobsthal
		for (size_t idx = pendingChain.size(); idx > 0; --idx) {
			if (!inserted[idx - 1]) {
				size_t boundaryIdx = getBoundary(mainChainIndices, idx-1);
				printInsertion(pendingChain[idx - 1], idx - 1, mainChain[boundaryIdx], boundaryIdx);
				lastInsertion = sortElementIntoChain(pendingChain[idx - 1], mainChain, mainChain.begin() + boundaryIdx);
				mainChainIndices.insert(mainChainIndices.begin() + (lastInsertion - mainChain.begin()), 0);
			}
		}
	}
	if (odd) {
		std::cout << "     Insert odd elem ";
		odd->print(0);
		std::cout << " into main chain" << std::endl;
		sortElementIntoChain(odd, mainChain, mainChain.end());
	}

	std::cout << "Sorted main chain: ";
	print(mainChain);
	return mainChain;
}

template<typename T>
bool PmergeMe::mergePairs(T &elements, T &rest) {
	if (elements.empty()) {
		return false;
	}

	T newElements;
	bool merged = false;

	for (size_t i = 0; i < elements.size(); ++i) {
		if (i + 1 < elements.size()) {
			newElements.push_back(merge(elements[i], elements[i + 1]));
			++i; // Skip the next element as it has been merged
			merged = true;
		} else if (elements.size() == 1) {
			newElements.push_back(elements[i]);
		} else {
			rest.push_back(elements[i]);
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
                                       T &pendingChain, std::shared_ptr<Element> &odd, T &rest) {
	bool isFirstPair = true;

	for (auto &elem: elements) {
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
	}

	if (!rest.empty() && rest.back()->_depth == mainChain.back()->_depth) {
		odd = rest.back();
		rest.pop_back();
	}
}

template<typename T>
typename T::iterator PmergeMe::sortElementIntoChain(const std::shared_ptr<Element> &elem, T &mainChain,
                                                    typename T::iterator endBoundary) {
	auto it = std::upper_bound(
		mainChain.begin(), endBoundary, elem, [](const std::shared_ptr<Element> &a, const std::shared_ptr<Element> &b) {
			++globalComparisonCount;
			return a->getMaxValue() < b->getMaxValue();
		});

	return mainChain.insert(it, elem);
}

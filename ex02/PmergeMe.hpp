#pragma once
#include <iostream>
#include <numeric>
#include <variant>
#include <vector>

struct Element;

inline int globalComparisonCount = 0;

inline bool printDebug = false;

using Pair = std::pair<std::shared_ptr<Element>, std::shared_ptr<Element> >;

struct Element {
	std::variant<int, Pair> _data;
	int _depth = 0;

	explicit Element(int data, int depth = 0);

	explicit Element(const std::shared_ptr<Element> &first, const std::shared_ptr<Element> &second, int depth);

	[[nodiscard]] int getMaxValue() const;

	void print(int i, bool overridePrint = false) const;

	static void printSwap(const std::shared_ptr<Element> &first, const std::shared_ptr<Element> &second);

	void sortElement();
};

std::shared_ptr<Element> makeElement(int data);

std::shared_ptr<Element> merge(const std::shared_ptr<Element> &first, const std::shared_ptr<Element> &second);

class PmergeMe {
	template<typename T>
	static void parseInput(T &container, const std::vector<int> &input);

	template<typename T>
	static void printAllElements(const T &elements, bool overridePrint = false);

	template<typename T>
	static void printChains(const T &mainChain, const T &pendingChain, const std::shared_ptr<Element> &odd,
	                        const T &rest);

	template<typename T>
	static void run(T &elements);

	template<typename T>
	static std::vector<size_t>::iterator updateMainElementPositions(T &mainChain,
	                                                                std::vector<size_t> & mainChainPositions,
	                                                                 typename T::iterator & lastInsertion);

	template<typename T>
	static void handleJacobsthalIndices(T &mainChain, T &pendingChain, std::vector<size_t> mainChainPositions,
	                                    const std::vector<size_t> &jacobIndices, std::string prefix);

	template<typename T>
	static void processPendingChain(T &mainChain, T &pendingChain);

	template<typename T>
	static void handleOddElement(T &mainChain, std::shared_ptr<Element> odd);

	static void printJacobsthalIndices(const std::vector<size_t> &jacobIndices);

	static void printOddInsertion(const std::shared_ptr<Element> &odd);

	static size_t getBoundary(const std::vector<size_t> &indices, size_t value);

	static void printInsertion(const std::shared_ptr<Element> &elem, size_t idx,
	                           const std::shared_ptr<Element> &boundaryElem,
	                           size_t boundaryIdx, const std::string &prefix = "     ");

	template<typename T>
	static T sort(T &elements, T &rest);

	template<typename T>
	static bool mergePairs(T &elements, T &rest);

	template<typename T>
	static void swapPairs(T &elements, T &rest);

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

	static int runVector(const std::vector<int> &input, bool _printDebug = false);

	static int runDeque(const std::vector<int> &input, bool _printDebug = false);

	static std::vector<size_t> generateJacobsthalIndices(size_t listSize);
};


template<typename T>
void PmergeMe::parseInput(T &container, const std::vector<int> &input) {
	for (const auto &value: input) {
		container.push_back(makeElement(value));
	}
}

template<typename T>
void PmergeMe::run(T &elements) {
	T rest = {};
	sort(elements, rest);
}

template<typename T>
std::vector<size_t>::iterator PmergeMe::updateMainElementPositions(T &mainChain,
                                                                   std::vector<size_t> &mainChainPositions, typename T::iterator &lastInsertion) {
	return mainChainPositions.insert(mainChainPositions.begin() + (lastInsertion - mainChain.begin()), 0);
}

template<typename T>
void PmergeMe::handleJacobsthalIndices(T &mainChain, T &pendingChain, std::vector<size_t> mainChainPositions,
                                       const std::vector<size_t> &jacobIndices, const std::string prefix) {
	auto lastInsertion = mainChain.begin();
	for (size_t idx: jacobIndices) {
		if (idx < pendingChain.size()) {
			size_t boundaryIdx = getBoundary(mainChainPositions, idx);
			printInsertion(pendingChain[idx], idx, mainChain[boundaryIdx], boundaryIdx, prefix);
			lastInsertion = sortElementIntoChain(pendingChain[idx], mainChain, mainChain.begin() + boundaryIdx);
			updateMainElementPositions(mainChain, mainChainPositions, lastInsertion);
		}
	}
}

template<typename T>
void PmergeMe::processPendingChain(T &mainChain, T &pendingChain) {
	if (pendingChain.empty()) return;

	// Generate map with positions of elements in main chain
	std::vector<size_t> mainChainPositions(mainChain.size());
	std::iota(mainChainPositions.begin(), mainChainPositions.end(), 0); // Initialize with 0, 1, 2, ...

	std::vector<size_t> jacobIndices;

	// If only 1 element, insert without Jacobsthal
	if (pendingChain.size() < 2) {
		jacobIndices.push_back(0);
	} else {
		jacobIndices = generateJacobsthalIndices(pendingChain.size());
		printJacobsthalIndices(jacobIndices);
	}
	std::string prefix = pendingChain.size() < 2 ? "S -> " : "J -> ";
	handleJacobsthalIndices(mainChain, pendingChain, mainChainPositions, jacobIndices, prefix);
}

template<typename T>
void PmergeMe::handleOddElement(T &mainChain, std::shared_ptr<Element> odd) {
	if (!odd) return;
	printOddInsertion(odd);
	sortElementIntoChain(odd, mainChain, mainChain.end());
}

template<typename T>
T PmergeMe::sort(T &elements, T &rest) {
	if (mergePairs(elements, rest)) {
		swapPairs(elements, rest);
		elements = sort(elements, rest);
	}

	T mainChain;
	T pendingChain;
	std::shared_ptr<Element> odd;

	// Split elements into main chain, pending chain, odd element and rest
	splitElementsIntoChains(elements, mainChain, pendingChain, odd, rest);

	if (printDebug) {
		std::cout << "-------------------------------------------" << std::endl;
		printChains(mainChain, pendingChain, odd, rest);
	}

	// Insert pending chain elements into main chain based on Jacobsthal indices (if only 1 element, insert without Jacobsthal)
	processPendingChain(mainChain, pendingChain);

	// Insert odd element into main chain (if exists)
	handleOddElement(mainChain, odd);

	if (printDebug) {
		std::cout << "Sorted main chain: ";
		printAllElements(mainChain);
	}
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
void PmergeMe::swapPairs(T &elements, T &rest) {
	for (const auto &element: elements) {
		element->sortElement();
	}
	printChains(elements, {}, nullptr, rest);
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

#pragma region Print functions
template<typename T>
void PmergeMe::printAllElements(const T &elements, bool overridePrint) {
	if (!printDebug && !overridePrint) return;
	for (size_t i = 0; i < elements.size(); ++i) {
		elements[i]->print(i);
	}
	std::cout << std::endl;
}

template<typename T>
void PmergeMe::printChains(const T &mainChain, const T &pendingChain, const std::shared_ptr<Element> &odd,
                           const T &rest) {
	if (!printDebug) return;
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

#pragma endregion

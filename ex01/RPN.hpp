
#pragma once
#include <string>

class RPN {
public:
	RPN() = delete;
	~RPN() = delete;
	RPN(const RPN &other) = delete;
	RPN &operator=(const RPN &other) = delete;

	static int evaluate(const std::string &expression);
};

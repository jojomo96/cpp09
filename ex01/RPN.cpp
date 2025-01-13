
#include "RPN.hpp"

#include <sstream>
#include <stack>

int RPN::evaluate(const std::string &expression) {
	// We'll use a stack container to evaluate the RPN expression
	std::stack<int> st;
	std::istringstream iss(expression);
	std::string token;

	while (iss >> token)
	{
		// Check if token is a single-digit number
		if (token.size() == 1 && (token[0] >= '0' && token[0] <= '9'))
		{
			// Push the integer value onto the stack
			st.push(token[0] - '0');
		}
		// Otherwise, check if it is a supported operator
		else if (token == "+" || token == "-" || token == "*" || token == "/")
		{
			// We need at least two values on the stack
			if (st.size() < 2)
				throw std::runtime_error("Error: not enough operands for operator");

			const int b = st.top(); // second operand
			st.pop();
			const int a = st.top(); // first operand
			st.pop();

			int result = 0;
			if (token == "+")
				result = a + b;
			else if (token == "-")
				result = a - b;
			else if (token == "*")
				result = a * b;
			else if (token == "/")
			{
				if (b == 0)
					throw std::runtime_error("Error: division by zero");
				result = a / b;
			}

			// Push the result back onto the stack
			st.push(result);
		}
		else
		{
			// Any invalid token (including parentheses) => error
			throw std::runtime_error("Error: invalid token");
		}
	}

	// If after processing all tokens, the stack doesn't have exactly 1 element, it's an error
	if (st.size() != 1)
		throw std::runtime_error("Error: invalid expression");

	// The single remaining value on the stack is the result
	return st.top();
}

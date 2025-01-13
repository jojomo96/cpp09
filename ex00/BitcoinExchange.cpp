
#include "BitcoinExchange.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>


bool BitcoinExchange::isValidDate(const std::string& date) {
	std::istringstream ss(date);
	std::tm tm = {};
	ss >> std::get_time(&tm, "%Y-%m-%d");
	if (ss.fail()) {
		return false;
	}

	// Check if the date is valid
	std::chrono::system_clock::time_point tp;
	try {
		tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
	} catch (...) {
		return false;
	}

	// Check if the parsed date matches the input date
	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d");
	return date == oss.str();
}

bool BitcoinExchange::isValidExchangeRate(const std::string& rate) {
	const std::regex ratePattern(R"(\d+(\.\d+)?)");
	return std::regex_match(rate, ratePattern);
}

std::map<std::string, double> BitcoinExchange::_parseExchangeRates() {
	std::ifstream exchangeRates(EXCHANGE_RATES_FILE);
	if (!exchangeRates) {
		std::cerr << "Error: Could not open file " << EXCHANGE_RATES_FILE << std::endl;
		return {};
	}

	if (exchangeRates.peek() == std::ifstream::traits_type::eof()) {
		std::cerr << "Error: File " << EXCHANGE_RATES_FILE << " is empty" << std::endl;
		return {};
	}

	std::map<std::string, double> data;
	std::string line;
	std::getline(exchangeRates, line);

	if (line != EXCHANGE_RATES_FILE_HEADER) {
		std::cerr << "Error: Invalid header in file " << EXCHANGE_RATES_FILE << std::endl;
		return {};
	}

	while (std::getline(exchangeRates, line)) {
		std::istringstream ss(line);
		std::string date, rateStr;
		if (!std::getline(ss, date, ',') || !std::getline(ss, rateStr)) {
			throw std::runtime_error("Error: Invalid line format");
		}

		if (!isValidDate(date) || !isValidExchangeRate(rateStr)) {
			throw std::runtime_error("Error: Invalid date or exchange rate format");
		}

		double rate = std::stod(rateStr);
		if (data.find(date) != data.end()) {
			throw std::runtime_error("Error: Duplicate date found");
		}

		data[date] = rate;
	}

	exchangeRates.close();
	return data;

}

void BitcoinExchange::printResult(const std::string &inputFileName) {
	std::map<std::string, double> exchangeRates;
	try {
		exchangeRates = _parseExchangeRates();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return;
	}

	std::ifstream inputFile(inputFileName);
	if (!inputFile) {
		std::cerr << "Error: Could not open file " << inputFileName << std::endl;
		return;
	}

	if (inputFile.peek() == std::ifstream::traits_type::eof()) {
		std::cerr << "Error: File " << inputFileName << " is empty" << std::endl;
		return;
	}

	std::string line;
	std::getline(inputFile, line);

	if (line != INPUT_FILE_HEADER) {
		std::cerr << "Error: Invalid header in file " << inputFileName << std::endl;
		return;
	}

	while (std::getline(inputFile, line)) {
		std::istringstream ss(line);
		std::string date, valueStr;
		size_t delimiterPos = line.find(" | ");
		if (delimiterPos == std::string::npos) {
			std::cerr << "Error: Invalid line format:" << std::endl;
			continue;
		}

		date = line.substr(0, delimiterPos);
		valueStr = line.substr(delimiterPos + 3); // 3 is the length of " | "

		if (!isValidDate(date)) {
			std::cerr << "Error: Invalid date format" << std::endl;
			continue;
		}

		double value;
		try {
			value = std::stod(valueStr);
		} catch (const std::exception& e) {
			std::cerr << "Error: Invalid value format" << std::endl;
			continue;
		}

		if (value < 0 || value > 1000) {
			std::cerr << "Error: Value must be between 0 and 1000" << std::endl;
			continue;
		}

		std::map<std::string, double>::const_iterator it;
		try {
			findExchangeRate(it, exchangeRates, date);
		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			continue;
		}

		std::cout << date << " => " << value << " = " << value * it->second;
		if (it->first != date) {
			std::cout << " (date used: " << it->first << ")";
		}
		std::cout << std::endl;
	}
}

void BitcoinExchange::findExchangeRate(std::map<std::string, double>::const_iterator &it, const std::map<std::string, double>& exchangeRates, const std::string& date) {
	it = exchangeRates.lower_bound(date);
	if (it == exchangeRates.begin() && it->first != date) {
		throw std::runtime_error("Error: Exchange rate not found for date " + date);
	}

	if (it == exchangeRates.end() || it->first != date) {
		--it;
	}

	if (it == exchangeRates.end() || it->first > date) {
		throw std::runtime_error("Error: Exchange rate not found for date " + date);
	}
}

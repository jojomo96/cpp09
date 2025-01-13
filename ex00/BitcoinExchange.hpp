#pragma once
#include <map>
#include <string>

#define EXCHANGE_RATES_FILE "./data.csv"
#define EXCHANGE_RATES_FILE_HEADER "date,exchange_rate"
#define INPUT_FILE_HEADER "date | value"

class BitcoinExchange {
	std::map<std::string, double> _exchangeRates;

	static bool isValidDate(const std::string &date);

	static bool isValidExchangeRate(const std::string &rate);

	static std::map<std::string, double> _parseExchangeRates();

	static void findExchangeRate(std::map<std::string, double>::const_iterator &it, const std::map<std::string, double> &exchangeRates, const std::
	                             string &date);

public:
	BitcoinExchange() = delete;

	~BitcoinExchange() = delete;

	BitcoinExchange(const BitcoinExchange &other) = delete;

	BitcoinExchange &operator=(const BitcoinExchange &other) = delete;

	static void printResult(const std::string &inputFileName);
};

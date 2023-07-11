#include "../../includes/Parser.hpp"

int Parser::getWordCount(std::string str) {
	unsigned long i = 0;
	int count = 0;

	for (; i < str.size(); i++) {
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r') {
			count++;
			while (i < str.size() && str[i] != ' ' && str[i] != '\t' && str[i] != '\n' &&
				   str[i] != '\r') {
				i++;
			}
		}
	}
	return count;
}
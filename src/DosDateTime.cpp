#include "DosDateTime.hpp"

DosDateTime::DosDateTime(uint16_t date, uint16_t time) {
	year = (date >> 9) % 0b10000000;		// 15~9  7bit (-1980)
	month = (date >> 5) % 0b10000;			// 8~5   4bit
	day = date % 0b100000;					// 4~0   5bit

	hour = (time >> 11) % 0b100000;			// 15~11 5bit
	minute = (time >> 5) % 0b1000000;		// 10~5  6bit
	second = (time % 0b100000) * 2;			// 4~0   5bit
}

string DosDateTime::to_s() {
	char str[32];
	sprintf(str, "%u/%u/%uT%u:%u:%uZ", year + 1980, month, day, hour, minute, second);
	return str;
}
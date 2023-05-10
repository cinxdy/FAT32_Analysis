#pragma once

#include <cstdint>
#include <string>
#include <string>
#include <sstream>
using namespace std;

class DosDateTime
{
public:
	DosDateTime(uint16_t date, uint16_t time = 0);
	string to_s();

	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
};
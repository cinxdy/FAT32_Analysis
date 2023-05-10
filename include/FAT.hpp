#pragma once

#include <sstream>
#include "byte_buffer2.hpp"
#include "fat32_enum.hpp"
using namespace std;

class Fat
{
public:
	Fat(uint8_t* buffer);
	~Fat();

	uint32_t get_fat_entry(int entry_num);
	ENTRY_KIND get_entry_kind(int entry_num);
	string to_s();

	int total_entry_cnt;
	uint32_t media_type;
	uint32_t partition_status;

private:
	uint32_t* Fat_table;

};

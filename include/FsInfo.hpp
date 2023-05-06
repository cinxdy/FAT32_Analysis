#pragma once
#include <string>
#include <sstream>
#include "byte_buffer2.hpp"
using namespace std;

class FsInfo
{
public:
	FsInfo(uint8_t* buffer);
	~FsInfo();
	uint32_t signature1;
	uint32_t signature2;
	uint32_t signature3;

	uint32_t num_of_free_cluster;
	uint16_t next_free_cluster;

	bool is_valid();
};
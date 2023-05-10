#pragma once
#include "byte_buffer2.hpp"
using namespace std;

class Cluster
{
	const int inode_size = 0x20;
public:
	Cluster(uint8_t* buffer, int size);
	~Cluster();

};
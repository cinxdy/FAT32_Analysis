#pragma once

#include <string>
#include <vector>
#include "fat32_enum.hpp"
#include "byte_buffer2.hpp"
#include "LFNNode.hpp"
#include "DosDateTime.hpp"

using namespace std;

class Inode
{
public:
	string file_path;
	string file_name;
	FILE_NAME_TYPE file_name_type;
	string extension;
	ATTR attr;
	
	string created_datetime;
	string last_accessed_datetime;
	string last_written_datetime;

	uint32_t file_size;
	uint32_t first_cluster;
	uint32_t offset;

	int32_t* content;
	vector<Inode*> children;
	bool not_active;

	void trim_space(string* text);

	FILE_NAME_TYPE get_file_name_type(string* file_name);
	Inode(uint8_t* buffer);
	~Inode();

	ATTR get_attr(uint8_t attr);
	bool has_child();
	void set_lfn(vector<LFNNode> lfnList);
	string to_s();
};


#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <stdio.h>

#include "byte_buffer2.hpp"
#include "BootRecord.hpp"
#include "FAT.hpp"
#include "INode.hpp"
#include "LFNNode.hpp"
#include "fat32_enum.hpp"

class FsFat32
{
	const int sector_size = 0x200;
	const int inode_size = 0x20;
public:
	FILE* fp;
	BootRecord* br;
	Fat* fat;
	Inode* root_node;
	Inode* node_hash;

public:
	FsFat32(FILE* fp);
	~FsFat32();
	void show_all(Inode* parent, vector<Inode*> visited);
	bool export_to(string path);
	bool build_file_system();

private:
	uint8_t* read_file_to_buffer(FILE* fp, int offset, int size);
	void expand_all();
	void expand_all(Inode* parent);
	bool expand(Inode* node);
	void show_node(Inode* node);

	Inode* get_node(Inode* node, vector<string> pathList);
};

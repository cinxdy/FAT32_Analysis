
#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>

#include "byte_buffer2.hpp"
#include "BootRecord.hpp"
#include "FAT.hpp"
#include "INode.hpp"
#include "LFNNode.hpp"
#include "FsInfo.hpp"
#include "fat32_enum.hpp"

class FsFat32
{
	static const unsigned int sector_size = 0x200;
	static const unsigned int inode_size = 0x20;
public:
	ifstream ifs;
	BootRecord* br;
	FsInfo* fs_info;
	Fat** fat;
	Inode* root_node;
	Inode* node_hash;

public:
	FsFat32(string file_name);
	~FsFat32();
	void show_all(Inode* parent, vector<Inode*> visited);
	bool export_to(string path);
	bool build_file_system();
	void close();

private:
	uint8_t* read_file_to_buffer(uint32_t offset, uint32_t size);
	bool expand_all();
	bool expand_all(Inode* parent);
	bool expand(Inode* node);
	void show_node(Inode* node);
	unsigned int get_cluster_offset(int cluster_num);
	Inode* get_node(Inode* node, vector<string> pathList);
};

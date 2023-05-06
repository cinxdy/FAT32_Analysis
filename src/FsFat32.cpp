#include <iostream>
#include "FsFat32.hpp"
using namespace std;

FsFat32::FsFat32(FILE* fp)
{
	this->fp = fp;

	this->br = NULL;
	this->fs_info = NULL;
	this->fat = NULL;
	this->root_node = NULL;
	this->node_hash = NULL;
}

FsFat32::~FsFat32()
{
	delete fp;
	fp = nullptr;

	delete br;
	br = nullptr;

	delete fs_info;
	fs_info = nullptr;

	delete[] fat;
	fat = nullptr;

	delete root_node;
	root_node = nullptr;

	delete[] node_hash;
	node_hash = nullptr;

}

bool FsFat32::build_file_system()
{
	try
	{
		// bootRecord
		uint8_t* buffer = read_file_to_buffer(fp, 0, sector_size);
		br = new BootRecord(buffer);
		delete buffer;

		// fsInfo
		buffer = read_file_to_buffer(fp, br->fsinfo_area_offset, sector_size);
		fs_info = new FsInfo(buffer);
		delete buffer;

		// fat1 & 2
		fat = new Fat*[2];
		for (int i = 0; i < 2; i++) {
			buffer = read_file_to_buffer(fp, br->fat_area_offset[i], br->fat_area_size);
			fat[i] = new Fat(buffer);
			delete buffer;
		}

		// rootNode
		buffer = read_file_to_buffer(fp, br->data_area_offset, inode_size);
		root_node = new Inode(buffer);
		delete buffer;

		return expand_all();
	}
	catch (exception e)
	{
		return false;
	}
};

uint8_t* FsFat32::read_file_to_buffer(FILE* fp, int offset, int size) {
	auto buffer = new uint8_t[size];

	fseek(fp, offset, SEEK_SET);
	if (!fread(buffer, size, 1, fp))
		throw new exception();

	return buffer;
}

bool FsFat32::expand_all() {
	return expand_all(root_node);
}

bool FsFat32::expand_all(Inode* parent)
{
	if (!parent->has_child()) 
		return true;

	if (!expand(parent)) 
		return false;

	for each (Inode * child in parent->children)
		if (!expand_all(child)) 
			return false;

	return true;
}

bool FsFat32::expand(Inode* node)
{
	auto offset = br->data_area_offset;
	switch (node->attr)
	{
	case ATTR::VOLUMN:
		offset += inode_size;
		break;
	case ATTR::DIR:
		offset += br->cluster_size * (node->first_cluster - br->root_directory_cluster);
		break;
	}
	fseek(fp, offset, SEEK_SET);

	uint8_t* buff = new uint8_t[inode_size];
	while (fread(buff, inode_size, 1, fp))
	{
		if (buff[0] == 0x00) break; // entry is the end
		if (buff[0] == 0x2E) continue; // entry is not active
		if (buff[0x0b] == 0x0f) // entry is LFN
		{
			vector<LFNNode> lfnList;
			int i = 0;
			while (true)
			{
				LFNNode lfn(buff);
				lfnList.push_back(lfn);
				fread(buff, sizeof(char), inode_size, fp);
				if (buff[0x0b] != 0x0f) break;
			}
			Inode* child = new Inode(buff);
			child->set_lfn(lfnList);
			node->children.push_back(child);
		}
		else
		{
			Inode* child = new Inode(buff);
			node->children.push_back(child);
		}
	}
	return true;
}

void FsFat32::show_node(Inode* node)
{
	if (node->attr != ATTR::ARCHIVE)
		printf("/%s", node->file_name.c_str());
	else
		printf("/%s.%s", node->file_name.c_str(), node->extension.c_str());
};

void FsFat32::show_all(Inode* parent, vector<Inode*> visited)
{
	int children_cnt = parent->children.size();

	// check visited
	bool visited_this = find(visited.begin(), visited.end(), parent) != visited.end();

	// if visited, done;
	if (visited_this)  return;

	show_node(parent);

	// children exist
	bool visited_children = true;
	int i = 0;

	// check visited children
	for (i = 0; i < children_cnt; i++)
	{
		visited_children = (find(visited.begin(), visited.end(), parent->children[i]) != visited.end());
		if (!visited_children) break;
	}

	// visited all the children or leaf node
	if (visited_children)
	{
		visited.push_back(parent);
		printf("\n");
		show_all(root_node, visited); // refresh
	}
	// not yet visited i-th child
	else
	{
		show_all(parent->children[i], visited);
	}

	return;
};

bool FsFat32::export_to(string path)
{
	int cur = 0;
	vector<string> pathList;

	string filename;
	while (cur < path.length())
	{
		int next = path.find('/', ++cur);
		filename = path.substr(cur, next - cur);
		// cout << "cur "<<cur<<"next "<<next << endl;
		// cout << title << endl;
		pathList.push_back(filename);
		cur = next;
		if (cur == string::npos) break;
	}

	// cout << pathList.size() << endl;
	int pos = filename.rfind('.');
	string title = filename.substr(0, pos);
	pathList[pathList.size() - 1] = title;

	Inode* leafNode = get_node(root_node, pathList);
	if (leafNode == NULL) return false;


	FILE* new_fp = fopen(filename.c_str(), "wb");
	uint8_t* buff = new uint8_t[br->cluster_size];
	uint32_t cluster_num = leafNode->first_cluster;
	uint32_t file_size = leafNode->file_size;

	while (true)
	{
		fseek(fp, br->data_area_offset + br->cluster_size * (cluster_num - br->root_directory_cluster), SEEK_SET);

		if (file_size >= br->cluster_size)
		{
			fread(buff, br->cluster_size, 1, fp);
			fwrite(buff, br->cluster_size, 1, new_fp);
		}
		else
		{
			fread(buff, file_size, 1, fp);
			fwrite(buff, file_size, 1, new_fp);
		}

		file_size -= br->cluster_size;
		cluster_num = fat[0]->get_next_entry(cluster_num);
		if (!cluster_num) break;
	}

	fclose(new_fp);
	return true;
};

Inode* FsFat32::get_node(Inode* node, vector<string> pathList)
{
	if (!node->file_name.compare(pathList[0])) 
		return NULL;

	pathList.erase(pathList.begin());
	if (pathList.size() == 0) return node;

	for (int i = 0; i < node->children.size(); i++)
	{
		Inode* child_node = get_node(node->children[i], pathList);
		if (child_node != NULL) 
			return child_node;
	}
}


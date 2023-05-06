#include "Inode.hpp"

Inode::Inode(uint8_t* buffer)
{
	ByteBuffer2 bb(buffer);

	// file_name
	file_name = bb.get_ascii(8);
	trim_space(&file_name);
	file_name_type = get_file_name_type(&file_name);

	// extension
	extension = bb.get_ascii(3);
	trim_space(&extension);

	// attr_kind
	auto attr_uint = bb.get_uint8();
	attr = get_attr(attr_uint);

	bb.skip(8);
	auto first_cluster_high = bb.get_int16_le();

	bb.skip(4);
	auto first_cluster_low = bb.get_int16_le();

	file_size = bb.get_uint32_le();

	first_cluster = first_cluster_high << 0x16 | first_cluster_low;
}

Inode::~Inode()
{

}

ATTR Inode::get_attr(uint8_t attr)
{
	switch (attr)
	{
	case 0x01: return ATTR::READ_ONLY;
	case 0x02: return ATTR::HIDDEN;
	case 0x04: return ATTR::SYSTEM;
	case 0x08: return ATTR::VOLUMN;
	case 0x0f: return ATTR::LFN;
	case 0x10: return ATTR::DIR;
	case 0x20: return ATTR::ARCHIVE;
	case 0x40: return ATTR::DEVICE;
	case 0x80: return ATTR::RESERVED;
	default: return ATTR::UNKNOWN;
	}
};

bool Inode::has_child()
{
	if (attr == ATTR::VOLUMN) return true;
	if (attr == ATTR::DIR) return true;
	if (attr == ATTR::ARCHIVE) return true;
	return false;
}

void Inode::set_lfn(vector<LFNNode> lfnList) {
	file_name = "";
	for (int i = lfnList.size() - 1; i >= 0; i--)
	{
		file_name.append(lfnList[i].title);
	}
	// cout << "INode> title: " << title << endl;
};

void Inode::trim_space(string* text) {
	// Trim space(0x20)
	auto npos = text->find_last_not_of(0x20);
	if (npos != string::npos)
		text->erase(npos + 1);
}

FILE_NAME_TYPE Inode::get_file_name_type(string* file_name)
{
	char special_char = file_name->front();
	if (special_char == 0x00) return FILE_NAME_TYPE::USED;
	if (special_char == 0xE5) return FILE_NAME_TYPE::DELETED;
	if (special_char == 0x05) file_name->replace(0, 1, 1, char(0xE5));

	return FILE_NAME_TYPE::NONE;
}

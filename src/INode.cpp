#include "Inode.hpp"
#include <iostream>

Inode::Inode(uint8_t* buffer, int size)
{
	ByteBuffer2 bb(buffer, size);

	// file_name
	file_name = bb.get_ascii(8);
	if (file_name[0] == 0x2E)
		not_active = true;

	trim_space(&file_name);
	file_name_type = get_file_name_type(&file_name);

	// extension
	extension = bb.get_ascii(3);
	trim_space(&extension);

	// attr_kind
	auto attr_uint = bb.get_uint8();
	attr = get_attr(attr_uint);

	bb.skip(2);
	auto created_time = bb.get_uint16_le();
	auto created_date = bb.get_uint16_le();
	created_datetime = DosDateTime(created_date, created_time).to_s();

	auto last_accessed_date = static_cast<time_t>(bb.get_uint16_le() << 0x10);
	last_accessed_datetime = DosDateTime(last_accessed_date).to_s();

	auto first_cluster_high = bb.get_int16_le();

	auto last_written_time = bb.get_uint16_le();
	auto last_written_date = bb.get_uint16_le();
	last_written_datetime = DosDateTime(last_written_date, last_written_time).to_s();

	auto first_cluster_low = bb.get_int16_le();
	file_size = bb.get_uint32_le();
	first_cluster = first_cluster_high << 0x10 | first_cluster_low;
	cout << to_s() << endl;
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

string Inode::to_s() {
	stringstream ss;
	ss << "-----Inode------" << endl;
	ss << "File Name: " << file_name << endl;
	ss << "File Extension: " << extension << endl;
	ss << "Created DateTime: " << created_datetime << endl;
	ss << "Last Accessed DateTime: " << last_accessed_datetime << endl;
	ss << "Last Written DateTime: " << last_written_datetime << endl;
	ss << "File Size: " << file_size << endl;
	ss << "First Cluster Num: " << first_cluster << endl;

	return ss.str();
}
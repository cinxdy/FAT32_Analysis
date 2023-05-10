#include "Fat.hpp"

Fat::Fat(uint8_t* buffer)
{
	ByteBuffer2 bb(buffer);
	auto size = _msize(buffer);

	total_entry_cnt = size / 4;
	Fat_table = new uint32_t[total_entry_cnt];

	for (int i = 0; i < total_entry_cnt; i++)
		Fat_table[i] = bb.get_uint32_le();

	media_type = get_fat_entry(0);
	partition_status = get_fat_entry(1);
};

//Fat_map = new uint32_t[total_entry_cnt];
//
//auto offset = 2;
//auto first_offset = offset;
//int i = 0;
//
//while (offset < total_entry_cnt)
//{
//	if (get_entry_kind(offset) == ENTRY_KIND::NOT_ALLOCATED)
//		break;
//
//	if (get_entry_kind(offset) == ENTRY_KIND::FILE_END)
//	{
//		Fat_map[i++] = first_offset;
//		offset++;
//		first_offset = offset;
//		continue;
//	}
//
//	offset++;
//}


Fat::~Fat() {
	delete[] Fat_table;
}

uint32_t Fat::get_fat_entry(int entry_num)
{
	return Fat_table[entry_num];
};

ENTRY_KIND Fat::get_entry_kind(int entry_num)
{
	uint32_t entry = get_fat_entry(entry_num);
	entry &= 0x0FFFFFFF; // Ignore the first half bytes

	if (entry == 0x0)								return ENTRY_KIND::NOT_ALLOCATED;
	if (0x0000002 <= entry && entry < 0xFFFFFF0)	return ENTRY_KIND::EXIST;
	if (0xFFFFFF0 <= entry && entry < 0xFFFFFF7)	return ENTRY_KIND::RESERVED;
	if (entry == 0xFFFFFF7)							return ENTRY_KIND::BAD_SECTOR;
	if (0xFFFFFF8 <= entry && entry <= 0xFFFFFFF)	return ENTRY_KIND::FILE_END;

	return ENTRY_KIND::UNKNOWN;
};

string Fat::to_s()
{
	stringstream sstream;

	for (int i = 2; i < total_entry_cnt; i++)
	{
		if (get_entry_kind(i) == ENTRY_KIND::FILE_END)
			sstream << i << endl << endl;
		if (get_entry_kind(i) == ENTRY_KIND::EXIST)
			sstream << i << " ";
	}

	/*for (int i = 0; i < total_entry_cnt; i++)
		sstream << Fat_map[i] << endl;;*/

	return sstream.str();
};


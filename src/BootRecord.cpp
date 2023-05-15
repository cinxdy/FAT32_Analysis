#include "BootRecord.hpp"

BootRecord::BootRecord(uint8_t* buffer, int size)
{
	ByteBuffer2 bb(buffer, size);

	// 0x00
	jump_boot_code = bb.get_uint24_le();
	oem_name = bb.get_ascii(8);
	bytes_per_sector = bb.get_uint16_le();
	sector_per_cluster = bb.get_uint8();
	reserved_sector_count = bb.get_uint16_le();

	// 0x10
	num_of_fats = bb.get_uint8();
	root_entry_count = bb.get_uint16_le();
	total_sector_16 = bb.get_uint16_le();
	media = bb.get_uint8();
	fat_size_16 = bb.get_uint16_le();
	sector_per_track = bb.get_uint16_le();
	num_of_heads = bb.get_uint16_le();
	hidden_sector = bb.get_uint32_le();

	// 0x20
	total_sector_32 = bb.get_uint32_le();
	fat_size_32 = bb.get_uint32_le();
	ext_flags = bb.get_uint16_le();
	file_sys_version = bb.get_uint16_le();
	root_directory_cluster = bb.get_uint32_le();

	// 0x30
	file_sys_info = bb.get_uint32_le();
	backup_boot_sec = bb.get_uint16_le();
	bb.skip(12); // reserved

	// 0x40
	drv_num = bb.get_uint8();
	bb.skip(1);
	boot_sig = bb.get_uint8();
	volumn_id = bb.get_uint32_le();
	volumn_label = bb.get_ascii(11);

	// 0x50
	file_system_type = bb.get_ascii(8);

	error_msg = bb.get_ascii(418);
	signature = bb.get_uint16_le();

	// Calculate
	cluster_size = bytes_per_sector * sector_per_cluster;
	reserved_area_size = bytes_per_sector * reserved_sector_count;
	fat_area_size = fat_size_32 * bytes_per_sector;

	fsinfo_area_offset = bytes_per_sector * file_sys_info;
	fat_area_offset[0] = reserved_area_size;
	fat_area_offset[1] = fat_area_offset[0] + fat_area_size;
	data_area_offset = fat_area_offset[1] + fat_area_size;
};

bool BootRecord::is_valid()
{
	if (root_entry_count == 0 && total_sector_16 == 0 && signature == 0xAA55)
		return true;
	return false;
}
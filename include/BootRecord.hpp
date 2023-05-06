#pragma once

#include <string>
#include <sstream>
#include "byte_buffer2.hpp"

using namespace std;

class BootRecord
{
public:
	BootRecord(uint8_t* buffer);
	bool is_valid();

	uint32_t jump_boot_code;
	string oem_name;
	uint16_t bytes_per_sector;
	uint8_t sector_per_cluster;
	uint16_t reserved_sector_count;

	uint8_t num_of_fats;
	uint16_t root_entry_count;
	uint16_t total_sector_16;
	uint8_t media;
	uint16_t fat_size_16;
	uint16_t sector_per_track;
	uint16_t num_of_heads;
	uint32_t hidden_sector;

	uint32_t total_sector_32;
	uint32_t fat_size_32;
	uint16_t ext_flags;
	uint16_t file_sys_version;
	uint32_t root_directory_cluster;

	uint32_t file_sys_info;
	uint16_t backup_boot_sec;

	uint8_t drv_num;
	uint8_t boot_sig;
	uint32_t volumn_id;
	string volumn_label;

	string file_system_type;

	string error_msg;
	uint16_t signature;

	uint32_t cluster_size;
	uint32_t reserved_area_size;
	uint32_t fsinfo_area_offset;

	uint32_t fat_area_offset[2];
	uint32_t fat_area_size;

	uint32_t data_area_offset;

};


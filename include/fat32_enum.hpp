#pragma once

enum class FILE_SYSTEM_KIND
{
	FAT32,
	EXT3,
	EXT4,
};

enum class ENTRY_KIND
{
	UNKNOWN = 0,
	NOT_ALLOCATED,
	EXIST,
	RESERVED,
	BAD_SECTOR,
	FILE_END,
};

enum class FILE_NAME_TYPE
{
	NONE,
	USED,
	DELETED
};

enum class ATTR : unsigned int
{
	UNKNOWN,
	READ_ONLY = 0x01,
	HIDDEN = 0x02,
	SYSTEM = 0x04,
	VOLUMN = 0x08,
	LFN = 0x0F,
	DIR = 0x10,
	ARCHIVE = 0x20,
	DEVICE = 0x40,
	RESERVED = 0x80,
};
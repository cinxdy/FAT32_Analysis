
#include "FsInfo.hpp"

FsInfo::FsInfo(uint8_t* buffer)
{
	ByteBuffer2 bb(buffer);
	signature1 = bb.get_uint32_le();
	bb.skip(30);
	signature2 = bb.get_uint32_le();
	num_of_free_cluster= bb.get_uint32_le();
	next_free_cluster = bb.get_uint32_le();
	bb.skip(12);
	signature3 = bb.get_uint16_le();
}
FsInfo::~FsInfo()
{
}

bool FsInfo::is_valid()
{
	if (signature1 != 0x52526141) return false;
	if (signature2 != 0x72724161) return false;
	if (signature3 != 0x55AA) return false;

	return true;
}

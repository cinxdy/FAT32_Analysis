#include "FAT.hpp"

FAT::FAT(uint8_t* buffer, int size)
{  
    ByteBuffer2 bb(buffer,size);
    auto total_entry_cnt = size/4;
    
    this->size = total_entry_cnt;

    FAT_table = new uint32_t[total_entry_cnt];
    for(int i=0;i<total_entry_cnt;i++)
        FAT_table[i] = bb.get_uint32_le();
};

uint32_t FAT::get_FAT_entry(int entry_num)
{
    return FAT_table[entry_num];
};

int FAT::get_size()
{
    return size;
};

uint32_t FAT::get_next_entry(int entry_num)
{
    if(get_entry_kind(entry_num)!=ENTRY_KIND::EXIST) return 0;
    return get_FAT_entry(entry_num);
};

ENTRY_KIND FAT::get_entry_kind(int entry_num)
{
    uint32_t entry = get_FAT_entry(entry_num);
    if(entry==0x0) return ENTRY_KIND::NOT_ALLOCATED;
    else if(entry>= 0x2 && entry < 0xffffff0) return ENTRY_KIND::EXIST;
    else if(entry>= 0xffffff0 && entry <= 0xffffff6 ) return ENTRY_KIND::RESERVED;
    else if(entry==0xffffff7) return ENTRY_KIND::BAD_SECTOR;
    else if(entry>= 0xffffff8 && entry <= 0xfffffff ) return ENTRY_KIND::FILE_END;
    else return ENTRY_KIND::UNKNOWN;
};

string FAT::to_s()
{
    stringstream sstream;
    string res="";
    for (int i=7;get_entry_kind(i)==ENTRY_KIND::EXIST;i++)
        sstream << i <<"th: " << hex << get_FAT_entry(i) << endl;
    res = sstream.str();
    return res;
};


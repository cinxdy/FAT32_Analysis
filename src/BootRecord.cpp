#include <sstream>
#include "byte_buffer2.hpp"
#include "BootRecord.hpp"

BootRecord::BootRecord(uint8_t* buffer, int size)
{
    ByteBuffer2 bb(buffer,size);
    bb.skip(11);
    bytes_per_sector        =bb.get_uint16_le();
    sector_per_cluster      =bb.get_uint8();
    reserved_sector_count   =bb.get_uint16_le();
    num_of_FAT              =bb.get_uint8();
    bb.skip(19);
    FAT_size_32             =bb.get_uint32_le();
    bb.skip(4);
    root_directory_cluster  =bb.get_uint32_le();

    cluster_size = bytes_per_sector*sector_per_cluster;

    reserved_area_size = bytes_per_sector* reserved_sector_count;

    //FAT Area Size = Num of FAT * (FAT Size 32 * Bytes Per Sector)
    FAT1_area_offset = reserved_area_size;
    FAT1_area_size = FAT_size_32*bytes_per_sector;

    // Data Area Offset = Reserved Area Size + FAT Area Size
    data_area_offset =  FAT1_area_offset + FAT1_area_size*2;
    // data_area_cluster_offset = data_area_offset;
};

string BootRecord::to_s()
{   
    stringstream sstream;
    string res="";
    // int i = 10;
    // char buf[1024] = { 0 };
    // sprintf(buf, "%x", i);
    // string res(buf);

    // sstream << hex << bytes_per_sector << endl;
    // sstream << hex << sector_per_cluster<< endl;
    // sstream << hex << reserved_sector_count<< endl;
    // sstream << hex << num_of_FAT<<endl;
    // sstream << hex << FAT_size_32<< endl;
    // sstream << hex << root_directory_cluster << endl;
    
    sstream << "Cluster size: " << hex << cluster_size << endl;
    sstream << "FAT1 offset: " << hex << FAT1_area_offset << endl;
    sstream << "Data offset: " << hex << data_area_offset << endl;
    
    res = sstream.str();

    return res;
};
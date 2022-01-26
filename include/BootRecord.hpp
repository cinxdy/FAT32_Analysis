#pragma once

#include <string>
using namespace std;

class BootRecord 
{
    public:

    uint16_t bytes_per_sector;
    uint8_t sector_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t num_of_FAT;
    uint32_t FAT_size_32;
    uint32_t root_directory_cluster;

    unsigned int cluster_size;
    unsigned int reserved_area_size;
    unsigned int FAT1_area_offset;
    unsigned int FAT1_area_size;
    unsigned int data_area_offset;
    unsigned int data_area_cluster_offset;


    BootRecord(uint8_t* buffer, int size);
    string to_s();
};

    
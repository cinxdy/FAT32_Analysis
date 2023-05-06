#pragma once

#include <sstream>
#include "byte_buffer2.hpp"
#include "fat32_enum.hpp"
using namespace std;

class Fat
{
    private:
    uint32_t* FAT_table;
    int size;

    public:

    Fat(uint8_t* buffer);
    uint32_t get_fat_entry(int entry_num);
    int get_size();
    uint32_t get_next_entry(int entry_num);
    ENTRY_KIND get_entry_kind(int entry_num);
    string to_s();

};

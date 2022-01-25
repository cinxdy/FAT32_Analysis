#include <sstream>
#include "byte_buffer2.hpp"
#include "fat32_enum.hpp"

class FAT
{
    private:
    uint32_t* FAT_table;
    int size;

    public:

    FAT(uint8_t* buffer, int size);
    uint32_t get_FAT_entry(int entry_num);
    int get_size();
    uint32_t get_next_entry(int entry_num);
    ENTRY_KIND get_entry_kind(int entry_num);
    string to_s();

};

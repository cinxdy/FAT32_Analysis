#pragma once

#include <string>
#include <vector>
#include "fat32_enum.hpp"
#include "byte_buffer2.hpp"
#include "LFNNode.hpp"
using namespace std;

class INode
{
    public:
    string title;
    string extension;
    uint8_t attr;
    uint32_t file_size;
    uint32_t first_cluster;
    uint32_t offset;
    vector<INode> children;

    INode(uint8_t* buffer, int size);
    ATTR get_ATTR();
    void set_lfn(vector<LFNNode> lfnList);
    string to_s();
};

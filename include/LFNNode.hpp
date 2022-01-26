#pragma once

#include <string>
#include "byte_buffer2.hpp"

using namespace std;
class LFNNode
{   
    public:
    string title;
    LFNNode(uint8_t* buffer, int size);
};
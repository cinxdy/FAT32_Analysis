#include "LFNNode.hpp"

LFNNode::LFNNode(uint8_t* buffer)
{
    ByteBuffer2 bb(buffer);
    bb.skip(1);
    string title1 = bb.get_ascii(5);
    bb.skip(3);
    string title2 = bb.get_ascii(6);
    bb.skip(2);
    string title3 = bb.get_ascii(2);

    title = title1+title2+title3;
    // cout << title << endl;
};
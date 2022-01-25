LFNNode::LFNNode(uint8_t* buffer, int size)
{
    ByteBuffer2 bb(buffer,size);
    bb.skip(1);
    string title1                   = bb.get_unicode16_le(5);
    bb.skip(3);
    string title2                   = bb.get_unicode16_le(6);
    bb.skip(2);
    string title3                   = bb.get_unicode16_le(2);

    title = title1+title2+title3;
    // cout << title << endl;
};
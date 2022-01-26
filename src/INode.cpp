#include "INode.hpp"
// using namespace std;

INode::INode(uint8_t* buffer, int size)
{
    ByteBuffer2 bb(buffer,size);

    title                   = bb.get_ascii(8);
    for(int i=title.length()-1;i>=0;i--)
    {
        if(title[i]==0x20) title.erase(i);
        else break;
    }

    extension               = bb.get_ascii(3);
    
    for(int i=extension.length()-1;i>=0;i--)
    {
        if(extension[i]==0x20) extension.erase(i);
        else break;
    }
    attr                    = bb.get_uint8();

    bb.skip(8);
    auto first_cluster_high = bb.get_int16_le();
    
    bb.skip(4);
    auto first_cluster_low  = bb.get_int16_le();

    file_size = bb.get_uint32_le();

    first_cluster = first_cluster_high << 0x16 | first_cluster_low;
 
};

ATTR INode::get_ATTR()
{
    switch(attr)
    {
        case 0x01: return ATTR::READ_ONLY;
        case 0x02: return ATTR::HIDDEN;
        case 0x04: return ATTR::SYSTEM;
        case 0x08: return ATTR::VOLUMN;
        case 0x0f: return ATTR::LFN;
        case 0x10: return ATTR::DIR;
        case 0x20: return ATTR::ARCHIVE;
        case 0x40: return ATTR::DEVICE;
        case 0x80: return ATTR::RESERVED;
        default: return ATTR::UNKNOWN;

    }
};

void INode::set_lfn(vector<LFNNode> lfnList){
    title="";
    for(int i=lfnList.size()-1;i>=0;i--)
    {
        title.append(lfnList[i].title) ;
    }

    // cout << "INode> title: " << title << endl;
};

string INode::to_s()
{
    string res="";
    res += "title:"+ title + "." + extension;
    return res;
    // char buf[1024] = { 0 };
    // sprintf(buf, "Title: %s.%s", title.c_str(), extension.c_str());
    // return buf;
};

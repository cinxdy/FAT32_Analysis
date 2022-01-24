#include "byte_buffer2.hpp"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>

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

    BootRecord(uint8_t* buffer, int size)
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
    }

    string to_s()
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

        // printf("%x %x %x %x %x %x \n",bytes_per_sector,sector_per_cluster,reserved_sector_count, num_of_FAT, FAT_size_32, root_directory_cluster);
        
        // printf("Cluster size: %x \n", cluster_size);
        // printf("FAT1 offset: %x \n", FAT1_area_offset);
        // printf("Data offset: %x \n", data_area_offset);
    }
};

class FAT
{
    private:
    uint32_t* FAT_table;
    int size;

    public:
     enum class ENTRY_KIND 
     {
            NOT_ALLOCATED,
            EXIST,
            RESERVED,
            BAD_SECTOR,
            FILE_END,
            UNKNOWN
        };

    FAT(uint8_t* buffer, int size)
    {  
        ByteBuffer2 bb(buffer,size);
        auto total_entry_cnt = size/4;
        
        this->size = total_entry_cnt;

        FAT_table = new uint32_t[total_entry_cnt];
        for(int i=0;i<total_entry_cnt;i++)
            FAT_table[i] = bb.get_uint32_le();
    }

    uint32_t get_FAT_entry(int entry_num)
    {
        return FAT_table[entry_num];
    };

    int get_size()
    {
        return size;
    }

    uint32_t get_next_entry(int entry_num)
    {
        if(get_entry_kind(entry_num)!=ENTRY_KIND::EXIST) return 0;
        return get_FAT_entry(entry_num);
    }

    ENTRY_KIND get_entry_kind(int entry_num)
    {
        uint32_t entry = get_FAT_entry(entry_num);
        if(entry==0x0) return ENTRY_KIND::NOT_ALLOCATED;
        else if(entry>= 0x2 && entry < 0xffffff0) return ENTRY_KIND::EXIST;
        else if(entry>= 0xffffff0 && entry <= 0xffffff6 ) return ENTRY_KIND::RESERVED;
        else if(entry==0xffffff7) return ENTRY_KIND::BAD_SECTOR;
        else if(entry>= 0xffffff8 && entry <= 0xfffffff ) return ENTRY_KIND::FILE_END;
        else return ENTRY_KIND::UNKNOWN;
    }

    string to_s()
    {
        stringstream sstream;
        string res="";
        for (int i=0;i<10;i++)
            sstream << i <<"th: " << hex << get_FAT_entry(i) << endl;
        res = sstream.str();
        return res;
    }

};
enum  ATTR
    {
        READ_ONLY,
        HIDDEN ,
        SYSTEM ,
        VOLUMN ,
        LFN ,
        DIR ,
        ARCHIVE ,
        DEVICE ,
        RESERVED ,
        UNKNOWN
    };
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

    INode(uint8_t* buffer, int size)
    {
        ByteBuffer2 bb(buffer,size);

        title                   = bb.get_ascii(8);
        // bb.debug_it();
        extension               = bb.get_ascii(3);
        printf("\n%s . %s\n", title.c_str(),extension.c_str());
        // cout<<extension<<endl;
        // bb.debug_it();
        attr                    = bb.get_uint8();

        bb.skip(8);
        auto first_cluster_high = bb.get_int16_le();
        
        bb.skip(4);
        auto first_cluster_low  = bb.get_int16_le();

        first_cluster = first_cluster_high << 0x16 | first_cluster_low;
        // cout << hex << first_cluster_high <<" "<< first_cluster_low<<endl;
        cout << "First Cluster: " << hex << first_cluster <<endl;
        // bb.debug_it();

        // if(get_ATTR()==ATTR::DIR) Children (new INode(buffer, size));
    }
    
    void set_child_offset(uint8_t new_offset){
        offset = new_offset;
    }

    ATTR get_ATTR()
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
    } 
    

    // string to_s(){
    //     char buf[1024] = { 0 };
        // sprintf(buf, "Title: %s . %x", title+'\0', extension+'\0');
    //     return buf;
    // }
};

// class DataList{
//     public:

// }

class FileSystem
{
    public:
    BootRecord *br;
    FAT *fat;
    INode *rootNode;
    FILE *fp;

    FileSystem(FILE *fp){
        this->fp = fp;
    }

    void BuildFileSystem()
    {
        int size = 0x60;
        uint8_t* buffer = new uint8_t[size];

        // Boot Record
        fread(buffer, size, 1, fp);
        br = new BootRecord(buffer, size);
        cout << br->to_s() << endl;
        delete[] buffer;

        // FAT
        buffer = new uint8_t[br->FAT1_area_size];
        fseek(fp, br->FAT1_area_offset, SEEK_SET);
        fread(buffer, br->FAT1_area_size, 1, fp);
        
        fat = new FAT(buffer, br->FAT1_area_size);
        // cout << fat->to_s() << endl;
        delete[] buffer;

        // Data Inode
        buffer = new uint8_t[32];
        fseek(fp, br->data_area_offset, SEEK_SET);
        fread(buffer, 32, 1, fp);
        rootNode = new INode(buffer, 32);
        // rootNode->set_child_offset(br->data_area_offset + br->cluster_size*(rootNode->first_cluster-br->root_directory_cluster));
        delete[] buffer;
    }

    bool expand(INode* node)
    {   cout<<"expand starts "<<node->title.c_str()<<endl;
        uint8_t buff[32]; 

        cout << "expand: ATTR:: " << node->get_ATTR() << endl;
        
        if(node->get_ATTR()==ATTR::ARCHIVE)
        {
            return false;
        }
        
        if(node->get_ATTR()==ATTR::VOLUMN)
        {   
            // fseek(fp, br->data_area_offset + br->cluster_size*(node->first_cluster - br->root_directory_cluster), SEEK_SET);
            fseek(fp, br->data_area_offset, SEEK_SET);
            fseek(fp, 32, SEEK_CUR);
        }
        else if(node->get_ATTR()==ATTR::DIR)
        {
            fseek(fp, br->data_area_offset + br->cluster_size*(node->first_cluster - br->root_directory_cluster), SEEK_SET);
        }
        else return false;

        while(true)
        {   
            // cout << "offset: " << hex<< br->data_area_offset  + br->cluster_size*(node->first_cluster - br->root_directory_cluster)<<endl;
            // cout << "file pointer offset: " << hex << fp->_ptr <<endl;

            fread(buff, 32, 1, fp);
            // for(int i=0;i<32;i++) cout<< hex<< buff[i] <<" ";
            if(buff[0]==0x00) break;

            INode child(buff,32);
            node->children.push_back(child);
            // cout << "push_back: succeed"<<endl;
            // child->set_child_offset(br->data_area_offset + br->cluster_size*(child->first_cluster-br->root_directory_cluster));
        }
        
        cout << "children_size: children_size:" << node->children.size() << endl;
        
        // else if(node->get_ATTR==ATTR::ARCHIVE)
        // {
        //     return 0;
        // }
        // else if(node->get_ATTR==ATTR::LFN)
        // {   
        //     fseek(fp, br->data_area_offset + br->cluster_size);
        //     fread(buff, 32, 1, fp);
            
        // }
        
        return true;
    }

    int expand_all(){
        return expand_all(rootNode);
    }

    int expand_all(INode* parent)
    {   
        cout << "expand_all: " << parent->title.c_str() << endl;
        bool dir = expand(parent);
        
        cout << "expand_all: children_size:" << parent->children.size() << endl;
        // cout <<dir<<endl;
        if(!dir) return 0;

        for (int i=0;i<parent->children.size();i++)
            expand_all(&parent->children[i]);
    }

    int getNode()
    {

    }
    
};



int main(int argc, char** argv)
{
    char* path;
    if(argc == 1) sprintf(path, "./FAT32_simple.mdf");
    else path = argv[1];
    
    FILE *fp = fopen(path,"r");
    FileSystem fs(fp);
    fs.BuildFileSystem();
    fs.expand_all();
    fclose(fp);
}
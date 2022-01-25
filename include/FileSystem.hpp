#include <vector>
#include <algorithm>
#include <string>
#include <stdio.h>

#include "byte_buffer2.hpp"
#include "BootRecord.hpp"
#include "FAT.hpp"
#include "INode.hpp"
#include "LFNNode.hpp"
#include "fat32_enum.hpp"

class FileSystem
{
    public:
    BootRecord *br;
    FAT *fat;
    INode *rootNode;
    FILE *fp;

    FileSystem(FILE *fp);
    bool BuildFileSystem();
    bool expand(INode* node);
    void expand_all();
    void expand_all(INode* parent);
    void show_node(INode* node);
    void show_all(INode* parent, vector<INode*> visited);
    bool export_to(string path);
    INode* get_node(INode* node, vector<string> pathList);
};

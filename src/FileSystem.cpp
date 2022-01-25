FileSystem::FileSystem(FILE *fp)
{
    this->fp = fp;
};

bool FileSystem::BuildFileSystem()
{
    try
    {
        int size = 0x60;
        uint8_t* buffer = new uint8_t[size];

        // Boot Record
        fread(buffer, size, 1, fp);
        br = new BootRecord(buffer, size);
        // cout << br->to_s() << endl;
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
    catch(int e)
    {
        return false;
    }
    return true;
};

bool FileSystem::expand(INode* node)
{   
    uint8_t buff[32]; 
    // cout << "expand> title: " << node->title.c_str() << endl;
    // cout << "expand> ATTR: " << node->get_ATTR() << endl;
    
    // if(node->get_ATTR()==ATTR::ARCHIVE) return false;
    
    if(node->get_ATTR()==ATTR::VOLUMN)
    {   
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
        fread(buff, 32, 1, fp);
        // cout << "What is buff[0] :" << hex << buff[0] << endl;
        if(buff[0]==0x00 ) break; // if entry is the end
        if(buff[0]==0x2E) continue; // if entry is not active

        if(buff[0x0b]==0x0f) // if entry is LFN
        {   
            vector<LFNNode> lfnList;
            int i=0;
            while(true)
            {   
                LFNNode lfn(buff,32);
                lfnList.push_back(lfn);
                fread(buff, 32, 1, fp);
                if(buff[0x0b]!=0x0f) break;
            }
            INode child(buff, 32);
            child.set_lfn(lfnList);
            node->children.push_back(child);
        }
        else
        {
            INode child(buff,32);
            node->children.push_back(child);
        }
    }
    return true;
};

void FileSystem::expand_all(){
    expand_all(rootNode);
}

void FileSystem::expand_all(INode* parent)
{   
    bool dir = expand(parent);
    // cout << "expand_all> title: " << parent->title.c_str() << endl;
    // cout << "expand_all> children_size: " << parent->children.size() << endl;
    // cout << "expand_all> dir true or false: " << dir <<endl;

    if(!dir) return ;

    for (int i=0;i<parent->children.size();i++)
        expand_all(&parent->children[i]);
};

void FileSystem::show_node(INode* node)
{
    if(node->get_ATTR()!=ATTR::ARCHIVE)
        printf("/%s",node->title.c_str());
    else 
        printf("/%s.%s", node->title.c_str(),node->extension.c_str());
};

void FileSystem::show_all(INode* parent, vector<INode*> visited)
{   
    int children_cnt = parent->children.size();

    // check visited
    bool visited_this = find(visited.begin(), visited.end(), parent) != visited.end();

    // if visited, done;
    if(visited_this)  return ;

    show_node(parent);

    // children exist
    bool visited_children=true;
    int i=0;

    // check visited children
    for(i=0;i<children_cnt;i++)
    {   
        visited_children = (find(visited.begin(), visited.end(), &parent->children[i]) != visited.end());
        if(!visited_children) break;
    }

    // visited all the children or leaf node
    if(visited_children)
    {
        visited.push_back(parent);
        printf("\n");
        show_all(rootNode, visited); // refresh
    }
    // not yet visited i-th child
    else
    {   
        show_all(&parent->children[i], visited);
    }

    return ;
};

bool FileSystem::export_to(string path)
{
    int cur = 0;
    vector<string> pathList;
    
    string filename;
    while(cur < path.length())
    {
        int next = path.find('/',++cur);
        filename = path.substr(cur, next-cur);
        // cout << "cur "<<cur<<"next "<<next << endl;
        // cout << title << endl;
        pathList.push_back(filename);
        cur = next;
        if(cur==string::npos) break;
    }

    // cout << pathList.size() << endl;
    int pos = filename.rfind('.');
    string title = filename.substr(0, pos);
    pathList[pathList.size()-1] = title;

    INode *leafNode = get_node(rootNode, pathList);
    if(leafNode==NULL) return false;
    
    
    FILE *new_fp = fopen(filename.c_str(), "wb");
    uint8_t buff[br->cluster_size];
    uint32_t cluster_num = leafNode->first_cluster;
    uint32_t file_size = leafNode->file_size;
    // cout << "file size" << hex << leafNode->file_size <<endl;
    while(true)
    {
        // cout << "cluster:" << hex << cluster_num << endl;
        cout << "location" << hex<<br->data_area_offset + br->cluster_size*(cluster_num - br->root_directory_cluster)<<endl;
        fseek(fp, br->data_area_offset + br->cluster_size*(cluster_num - br->root_directory_cluster), SEEK_SET);
        
        if(file_size >= br->cluster_size)
        {
            fread(buff, br->cluster_size, 1, fp);
            fwrite(buff, br->cluster_size, 1, new_fp);
        }
        else
        {
            fread(buff, file_size, 1, fp);
            fwrite(buff, file_size, 1, new_fp);
        }
            
        file_size -= br->cluster_size;
        cluster_num = fat->get_next_entry(cluster_num);
        if(!cluster_num) break;
    }

    fclose(new_fp);
    return true;
};

INode* FileSystem::get_node(INode* node, vector<string> pathList)
{
    if(node->title.compare(pathList[0])==0)
    {
        pathList.erase(pathList.begin());
        if(pathList.size()==0) return node;
        else 
        {   
            for(int i=0;i<node->children.size();i++)
            {
                INode* child_node = get_node(&node->children[i],pathList);
                if(child_node != NULL) return child_node;
            }
                
        }
    }
    return NULL;
}

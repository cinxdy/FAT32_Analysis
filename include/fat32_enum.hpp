enum ENTRY_KIND 
{
    UNKNOWN=0,
    NOT_ALLOCATED,
    EXIST,
    RESERVED,
    BAD_SECTOR,
    FILE_END,
};


enum  ATTR
{
    UNKNOWN=0,
    READ_ONLY,
    HIDDEN ,
    SYSTEM ,
    VOLUMN ,
    LFN ,
    DIR ,
    ARCHIVE ,
    DEVICE ,
    RESERVED ,
};
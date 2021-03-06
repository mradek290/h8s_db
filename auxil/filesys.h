
#ifndef H8SDB_FILESYS_HEADER
#define H8SDB_FILESYS_HEADER

#ifndef COMFY_STRING_HEADER
#error "Comfystring required but not included.\n Get it from https://github.com/mradek290/comfystring.git"
#endif

typedef struct {
    char* AbsolutePath;
    char* Name;
    char* Extension;
    unsigned long long Size;
    _Bool IsFile;
} h8sFileInfo;

h8sFileInfo* h8sEnumerateDirectory( const char*, unsigned* );
void h8sFreeDirectoryEnumeration( h8sFileInfo*, unsigned );

#endif

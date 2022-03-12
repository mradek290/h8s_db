
#ifndef H8SDB_FILESYS_IMPL
#define H8SDB_FILESYS_IMPL

char* h8s__PrepDir( const char* dir ){

    unsigned dir_sz = strlen(dir);

    if( dir_sz >= 2 && !memcmp( dir + dir_sz - 2, "\\*", 3) ){

        return cfstrCreateSz( dir, dir_sz );

    }else if( dir_sz >= 1 && !memcmp( dir + dir_sz - 1, "\\", 2 ) ){

        unsigned* adr = (unsigned*) malloc( sizeof(unsigned) + dir_sz + 2 );
        char* s = (char*)(adr+1);

        *adr = dir_sz + 1;
        memcpy(s, dir, dir_sz );
        s[dir_sz++] = '*';
        s[dir_sz] = 0;

        return s;

    }else{

        unsigned* adr = (unsigned*) malloc( sizeof(unsigned) + dir_sz + 3 );
        char* s = (char*)(adr+1);

        *adr = dir_sz + 2;
        memcpy(s, dir, dir_sz );
        s[dir_sz++] = '\\';
        s[dir_sz++] = '*';
        s[dir_sz] = 0;

        return s;
    }
}

h8sFileInfo* h8sEnumerateDirectory( const char* dir, unsigned* elements ){

    if( cfstrNullOrEmpty(dir) ){
        *elements = 0;
        return 0;
    }

    char* path = h8s__PrepDir(dir);
    unsigned path_sz = cfstrSize(path);
    WIN32_FIND_DATA fdata;
    HANDLE search = FindFirstFile( path, &fdata);

    if( search == INVALID_HANDLE_VALUE ){
        h8sLog( h8slog_Error, "Cannot enumerate following directory: ");
        h8sLog( h8slog_Error, dir );
        cfstrFree(path);
        *elements = 0;
        return 0;
    }

    h8sLog( h8slog_Debug, "Enumerating following directory: " );
    h8sLog( h8slog_Debug, dir );

    unsigned counter = 0;
    do{

        unsigned fname_sz = strlen(fdata.cFileName);
        _Bool wantskip[] = {
            fname_sz == 1 && fdata.cFileName[0] == '.',
            fname_sz == 2 && fdata.cFileName[0] == '.' && fdata.cFileName[1] == '.'
        };

        if( !(wantskip[0] || wantskip[1]) ){
            ++counter;
        }

    }while( FindNextFile( search, &fdata ) );
    FindClose(search);

    h8sFileInfo* nfo0 = (h8sFileInfo*) malloc( sizeof(h8sFileInfo) * counter );
    h8sFileInfo* nfo1 = nfo0;
    search = FindFirstFile(path, &fdata);
    do{

        unsigned fname_sz = strlen(fdata.cFileName);
        _Bool wantskip[] = {
            fname_sz == 1 && fdata.cFileName[0] == '.',
            fname_sz == 2 && fdata.cFileName[0] == '.' && fdata.cFileName[1] == '.'
        };

        if( !(wantskip[0] || wantskip[1]) ){
            
            nfo1->Name = cfstrCreateSz( fdata.cFileName, fname_sz);

            if( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ){

                nfo1->IsFile = 0;
                nfo1->Size = 0;

                unsigned abspath_sz = path_sz + fname_sz + 1;
                unsigned* adr = (unsigned*) malloc( sizeof(unsigned) + abspath_sz);
                char* abspath = (char*)(adr+1);
                memcpy( abspath, path, path_sz );
                memcpy( abspath + path_sz-1, fdata.cFileName, fname_sz );
                abspath[abspath_sz-2] = '\\';
                abspath[abspath_sz-1] = 0;

                *adr = abspath_sz;
                nfo1->AbsolutePath = abspath;
                nfo1->Extension = 0;

            }else{

                nfo1->IsFile = 1;
                nfo1->Size = (((unsigned long long)fdata.nFileSizeHigh) << 32) | fdata.nFileSizeLow;

                unsigned abspath_sz = path_sz + fname_sz;
                unsigned* adr = (unsigned*) malloc( sizeof(unsigned) + abspath_sz );
                char* abspath = (char*)(adr+1);

                memcpy( abspath, path, path_sz );
                memcpy( abspath + path_sz-1, fdata.cFileName, fname_sz );
                abspath[abspath_sz-1] = 0;

                *adr = abspath_sz;
                nfo1->AbsolutePath = abspath;

                char* dot = strrchr(fdata.cFileName, '.' );
                if( dot ){
                    unsigned dot_sz = fname_sz - ( dot - fdata.cFileName );
                    nfo1->Extension = cfstrCreateSz( dot, dot_sz );
                }
            }

            ++nfo1;
        }

    }while( FindNextFile( search, &fdata ) );
    FindClose(search);

    cfstrFree(path);
    *elements = counter;
    return nfo0;
}

void h8sFreeDirectoryEnumeration( h8sFileInfo* info, unsigned elements ){

    if( !info || !elements ){
        h8sLog( h8slog_Debug, "Trying to free invalid directory enumeration.");
    }

    while( elements-- > 0 ){

        cfstrFree(info[elements].AbsolutePath);
        cfstrFree(info[elements].Name);
        if( info[elements].Extension )
            cfstrFree(info[elements].Extension);
    }

    free(info);
}

#endif

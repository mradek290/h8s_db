
#ifndef H8SDB_LOGGING_IMPL
#define H8SDB_LOGGING_IMPL

#ifndef COMFY_STRING_HEADER
#error "Comfystring required but not included.\n Get it from https://github.com/mradek290/comfystring.git"
#endif

unsigned h8slog__counter;
HANDLE h8slog__stout;

void h8sInitLog(){

    h8slog__counter = 0;
    h8slog__stout = GetStdHandle(STD_OUTPUT_HANDLE);
}

const char* h8sGetLogTypeName( h8sLogType x ){
    switch(x){
        case h8slog_Info      : return "Information";
        case h8slog_Error     : return "Error";
        case h8slog_Warning   : return "Warning";
        case h8slog_Invariant : return "Invariant";
        default : return "Invalid Logging Type!";
    }
}

void h8sLog( h8sLogType logtype, const char* msg ){

    if( cfstrNullOrEmpty(msg) ) return;

    unsigned msg_sz = strlen(msg);
    unsigned buf_sz = msg_sz + 200;
    char* buf = (char*) malloc(buf_sz);

    SYSTEMTIME st;
    GetSystemTime(&st);
    
    DWORD dummy;
    unsigned output_sz = sprintf(
        buf,
        "[%0.5u%12s %0.2d.%0.2d.%0.4d %0.2d:%0.2d:%0.2d.%0.3d] %s\r\n",
        h8slog__counter++, h8sGetLogTypeName(logtype),
        st.wDay, st.wMonth, st.wYear,
        st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
        msg
    );

    WriteFile(
        h8slog__stout, buf, output_sz, &dummy, 0
    );

    free(buf);
}

#endif

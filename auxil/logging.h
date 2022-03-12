
#ifndef H8SDB_LOGGING_HEADER
#define H8SDB_LOGGING_HEADER

extern unsigned h8sLogCounter;

typedef enum {
    h8slog_Info,
    h8slog_Error,
    h8slog_Warning,
    h8slog_Debug
} h8sLogType;

void h8sInitLog();
void h8sLog( h8sLogType, const char* );
const char* h8sGetLogTypeName( h8sLogType );

#endif

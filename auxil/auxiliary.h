
#ifndef H8SDB_AUXIL_HEADER
#define H8SDB_AUXIL_HEADER

#include <stdio.h>

#include "filesys.h"
#include "logging.h"

#ifdef H8SDB_WINDOWS

#include <windows.h>
#include "win/filesys.c"
#include "win/logging.c"

#endif

#endif

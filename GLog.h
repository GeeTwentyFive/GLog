/*
Copyright (c) 2025 Amar Alic

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _GLOG_H
#define _GLOG_H

#include <time.h>
#include <stdio.h>
#include <stdarg.h>



int _GLog__bInitialized = 0;
char _GLog__log_file_path[4096];
FILE* _GLog__log_file;



typedef enum {
        GLog__SUCCESS,

        // GLog__Init() errors:
        GLog__ERROR_ALREADY_INITIALIZED,
        GLog__ERROR_FILE_PATH_TOO_LONG,
        GLog__ERROR_FAILED_TO_OPEN_LOG_FILE,

        // GLog__Log() errors:
        GLog__ERROR_NOT_INITIALIZED,
        GLog__ERROR_FAILED_TO_WRITE_TIMESTAMP_TO_LOG_FILE,
        GLog__ERROR_FAILED_TO_WRITE_LOG_TO_LOG_FILE
} GLog__Error;







GLog__Error GLog__Init(char* log_path) {

        if (_GLog__bInitialized) return GLog__ERROR_ALREADY_INITIALIZED;



        time_t unix_time = time(NULL);
        struct tm* local_time = localtime(&unix_time);

        if (snprintf(_GLog__log_file_path, sizeof(_GLog__log_file_path),
                "%s__%d-%d-%d_%d-%d-%d",
                log_path,
                local_time->tm_year+1900,
                local_time->tm_mon+1,
                local_time->tm_mday,
                local_time->tm_hour,
                local_time->tm_min,
                local_time->tm_sec
        ) >= sizeof(_GLog__log_file_path)) return GLog__ERROR_FILE_PATH_TOO_LONG;

        _GLog__log_file = fopen(_GLog__log_file_path, "w");
        if (_GLog__log_file == NULL) return GLog__ERROR_FAILED_TO_OPEN_LOG_FILE;



        _GLog__bInitialized = 1;



        return GLog__SUCCESS;

}



#define GLog__Log(...) _GLog__Log(__FILE__, __LINE__, __VA_ARGS__)

GLog__Error _GLog__Log(char* current_file, int current_line, char *fmt, ...) {

        if (!_GLog__bInitialized) return GLog__ERROR_NOT_INITIALIZED;



        time_t unix_time = time(NULL);
        struct tm* local_time = localtime(&unix_time);

        if (fprintf(_GLog__log_file,
                "%d:%d:%d %s:%d: ",
                local_time->tm_hour,
                local_time->tm_min,
                local_time->tm_sec,
                current_file,
                current_line
        ) < 0) return GLog__ERROR_FAILED_TO_WRITE_TIMESTAMP_TO_LOG_FILE;



        va_list va;
        va_start(va, fmt);

        if (vfprintf(_GLog__log_file, fmt, va) < 0) return GLog__ERROR_FAILED_TO_WRITE_LOG_TO_LOG_FILE;

        va_end(va);



        fputc('\n', _GLog__log_file);



        return GLog__SUCCESS;

}



#endif // _GLOG_H
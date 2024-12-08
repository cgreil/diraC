//
// Created by christoph on 29.10.24.
//

#ifndef LEIBNITZ_LOGGER_H
#define LEIBNITZ_LOGGER_H

#include <stdio.h>
#include <stddef.h>
#include <common/string.h>

typedef enum LOGLEVEL {
    DEBUG,
    INFO
}LOGLEVEL;

typedef enum LOGOUTPUT {
    STDERR = 0,
    STDOUT = 1,
    LOGFILE = 2
}LOGOUTPUT;

typedef struct Logger {
    FILE* logstream;
    int fd;
    char* loggerName;
    size_t nameSize;
}Logger;

Logger* logger_create(LOGOUTPUT output);

size_t logger_appendString(Logger* log, String logstring);

size_t logger_appendCharArray(Logger* log, char* logstring, size_t length);

bool logger_attachStringBuilder(Logger* log, StringBuilder* stringBuilder);

bool logger_setLoglevel(Logger* log, LOGLEVEL newLevel);


#endif //LEIBNITZ_LOGGER_H

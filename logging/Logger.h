//
// Created by christoph on 29.10.24.
//

#ifndef LEIBNITZ_LOGGER_H
#define LEIBNITZ_LOGGER_H

#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "common/string.h"

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
    int fd;
    LOGLEVEL loglevel;
}Logger;

Logger* logger_create(LOGOUTPUT output);

void logger_log(Logger* log, LOGLEVEL loglevel, char* msg);

void logger_destroy(Logger* logger);


#endif //LEIBNITZ_LOGGER_H

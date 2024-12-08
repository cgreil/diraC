//
// Created by christoph on 30.10.24.
//

#include <fcntl.h>
#include "Logger.h"

Logger* logger_create(LOGOUTPUT output) {

    Logger* logger = malloc(sizeof(Logger));

    switch (output) {

        case STDERR: {
            char name[] = "Console Error Logger";
            logger->loggerName = name;
            logger->nameSize = sizeof(name);
            logger->fd = STDERR;
            break;
        }
        case STDOUT: {
            char name[] = "Console Info Logger";
            logger->loggerName = name;
            logger->nameSize = sizeof(name);
            logger->fd = STDOUT;
            break;
        }
        case LOGFILE: {
                // TODO: use malloc here instead
                char name[] = "File Logger";
                logger->loggerName = name;
                logger->nameSize = sizeof(name);
                // initialize file
                char filePath[] = "/logs/logs.txt";
                int flags = O_CREAT | O_APPEND;
                int fd = open(filePath, flags);
                // check for error
                if (fd == -1) {
                    fprintf(stderr, "Could not open or create logfile \n");
                    return NULL;
                }
        }
    }

    return logger;
}

void logger_destroy(Logger* logger) {

    if (logger == NULL) {
        return;
    }

    free(logger);
}

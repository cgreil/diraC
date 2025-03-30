//
// Created by christoph on 30.10.24.
//

#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#include "Logger.h"


#define stringBuilder_appendObject(_stringBuilder, T)       \
    _Generic((T),                                           \
    Vector: stringBuilder_appendVector,                     \
    Matrix: stringBuilder_appendMatrix,                     \
    Complex: stringBuilder_appendComplex,                   \
    VectorCollection: stringBuilder_appendVectorSet,        \
    String: stringBuilder_appendString                      \
    )                                                       \
    ( (_stringBuilder), (T) )



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


void logger_log(Logger* logger, LOGLEVEL loglevel, char* msg) {

    size_t message_length = strlen(msg);
    String messageStringSTR = string_create(msg, message_length);

    //get time and convert to string
    time_t currentTime;
    currentTime = time(NULL);
    struct tm* currentTimeTM;
    currentTimeTM = localtime(&currentTime);

    char* timeString;
    timeString = asctime(currentTimeTM);
    String timeStringSTR = string_create(timeString, strlen(timeString));

    // get string for Loglevel
    char* levelString = loglevel == DEBUG ? "DEBUG" : "INFO";
    String levelStringSTR = string_create(levelString, strlen(levelString));

    StringBuilder* stringBuilder = stringBuilder_create();
    stringBuilder_appendObject(stringBuilder, timeStringSTR);
    stringBuilder_appendCharArray(stringBuilder, ":\t", 2);
    stringBuilder_appendObject(stringBuilder, levelStringSTR);
    stringBuilder_appendCharArray(stringBuilder, ":\t", 2);
    stringBuilder_appendObject(stringBuilder, messageStringSTR);
    stringBuilder_appendCharArray(stringBuilder, "\n", 1);

    String outputString = stringBuilder_build(stringBuilder);

    write(logger->fd, outputString.data, outputString.length);
}




void logger_destroy(Logger* logger) {

    if (logger == NULL) {
        return;
    }

    free(logger);
}

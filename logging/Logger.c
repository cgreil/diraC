//
// Created by christoph on 30.10.24.
//

#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#include "Logger.h"

// ANSI escape codes for colors
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define stringBuilder_appendObject(_stringBuilder, T)       \
    _Generic((T),                                           \
    Vector: stringBuilder_appendVector,                     \
    Matrix: stringBuilder_appendMatrix,                     \
    Complex: stringBuilder_appendComplex,                   \
    VectorCollection: stringBuilder_appendVectorSet,        \
    String: stringBuilder_appendString                      \
    )                                                       \
    ( (_stringBuilder), (T) )


static String getLoggingTimeString() {

    //get time and convert to string
    time_t currentTime;
    currentTime = time(NULL);
    struct tm* currentTimeTM;
    currentTimeTM = localtime(&currentTime);

    // allocate large enough char array for time string
    char timeString[30];
    size_t formatSize = sizeof(timeString) / sizeof(char);
    //timeString = asctime(currentTimeTM);
    strftime(timeString, formatSize, "%a, %d-%m-%y: %H:%M:%S \t", currentTimeTM);

    
    return string_create(timeString, strlen(timeString));
}

static String getLoglevelString(LOGLEVEL loglevel) {

    char* str;    
    if (loglevel == DEBUG) {
        str = "[DEBUG]: ";
    } else if (loglevel == INFO) {
        str = "[INFO]: ";
    } else {
        str = "[ERROR]: ";
    }
    return string_create(str, strlen(str));
}


Logger* logger_init(LOGOUTPUT output) {

    logger = malloc(sizeof(Logger));

    switch (output) {

        case STDERR: {
            logger->fd = STDERR;
            break;
        }
        case STDOUT: {
            logger->fd = STDOUT;
            break;
        }
        case LOGFILE: {
                // TODO: use malloc here instead
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

void logger_logAll(LOGLEVEL loglevel, size_t numArgs, ...) {

#ifdef DEBUG_BUILD
    if (loglevel != DEBUG) {
        return;
    }
#endif


    StringBuilder* stringBuilder = stringBuilder_create();

    // start parsing vargs
    va_list logObjects;
    va_start(logObjects, numArgs);

    // retrieve next logObject and parse the internal union type
    LogObject logObj = va_arg(logObjects, LogObject);

    // select color based on loglevel
    if (loglevel == DEBUG) {
        stringBuilder_appendCharArray(stringBuilder, ANSI_COLOR_YELLOW, strlen(ANSI_COLOR_YELLOW));
    } else if (loglevel == INFO) {
        stringBuilder_appendCharArray(stringBuilder, ANSI_COLOR_CYAN, strlen(ANSI_COLOR_CYAN));
    } else if (loglevel == ERROR) {
        stringBuilder_appendCharArray(stringBuilder, ANSI_COLOR_RED, strlen(ANSI_COLOR_RED));
    }

    stringBuilder_appendString(stringBuilder, getLoglevelString(loglevel));
    stringBuilder_appendCharArray(stringBuilder, ANSI_COLOR_RESET, strlen(ANSI_COLOR_RESET));
    stringBuilder_appendString(stringBuilder, getLoggingTimeString());


    for (size_t argIndex = 0; argIndex < numArgs; argIndex++) {
        switch (logObj.type) {
            case STRING: {
                // use memcpy to get around typecasting rules for union members
                String str;
                memcpy(&str, &logObj.object, sizeof(String));
                stringBuilder_appendString(stringBuilder, str);
                break;                
            }
 
            case CHARS: {
                // use string type also for the case of null-terminated char array
                String str = string_create((char *) &(logObj.object), strlen((char *) &(logObj.object)));
                stringBuilder_appendString(stringBuilder, str);
                break;
            }

            case COMPLEX: {
                Complex compl;
                memcpy(&compl, &logObj.object, sizeof(Complex));
                stringBuilder_appendComplex(stringBuilder, compl);
                break; 
            }

            case VECTOR: {
                Vector vec;
                memcpy(&vec, &logObj.object, sizeof(Vector));
                stringBuilder_appendVector(stringBuilder, vec);
                break;
            }

            case MATRIX: {
                Matrix mat;
                memcpy(&mat, &logObj.object, sizeof(Matrix));
                stringBuilder_appendMatrix(stringBuilder, mat);
                break;
            }

            case VECTOR_COLLECTION: {
                VectorCollection vc;
                memcpy(&vc, &logObj.object, sizeof(VectorCollection));
                stringBuilder_appendVectorSet(stringBuilder, vc);
                break;
            }
        
            case NDARRAY: {
                NDArray ndarray;
                memcpy(&ndarray, &logObj.object, sizeof(ndarray));
                // todo: implement stringBuilder_appendNDarray
                //stringBuilder_append
                break; 
            }

            case QUREG: {
                QuantumRegister qureg;
                memcpy(&qureg, &logObj.object, sizeof(QuantumRegister));
                stringBuilder_appendQuantumRegister(stringBuilder, qureg);
                break;    
            }       
        
            default: {
                stringBuilder_appendString(stringBuilder, STR("Encountered type not yet implemented in stringBuilder"));
                break;
            }
        }              
    }
    va_end(logObjects);

    // finally create the String and write to output 
    String outputString = stringBuilder_build(stringBuilder);
    write(logger->fd, outputString.data, outputString.length);
}

void logger_destroy(Logger* logger) {

    if (logger == NULL) {
        return;
    }

    free(logger);
}
